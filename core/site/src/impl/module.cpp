#include "module.hpp"
#include <dci/site/error.hpp>
#include <dci/logger/logger.hpp>
#include <dci/async/functions.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>

#include <dlfcn.h>

namespace dci { namespace site { namespace impl
{
    std::string Module::generateManifest(const std::string &mainBinaryFullPath)
    {
        void *mainBinaryHandle = dlopen(mainBinaryFullPath.c_str(), RTLD_LAZY|RTLD_LOCAL|RTLD_NODELETE);

        if(!mainBinaryHandle)
        {
            std::cerr << dlerror();
            return std::string();
        }

        void **ppe = static_cast<void **>(dlsym(mainBinaryHandle, "dciModuleEntry"));
        if(!ppe)
        {
            dlclose(mainBinaryHandle);
            std::cerr<<"loading module \""<<mainBinaryFullPath<<"\": entry point is absent";
            return std::string();
        }

        ModuleEntry *entry = static_cast<ModuleEntry*>(*ppe);

        if(!entry)
        {
            dlclose(mainBinaryHandle);
            std::cerr<<"loading module \""<<mainBinaryFullPath<<"\": entry point is damaged";
            return std::string();
        }

        const ModuleInfo &info = entry->getInfo();

        boost::property_tree::ptree pt;
        {
            pt.add("provider", info._provider);
            pt.add("id", info._id.toHex());

            if(!info._serviceIds.empty())
            {
                boost::property_tree::ptree vals;
                for(const auto &v : info._serviceIds)
                {
                    vals.push_back(std::make_pair("", boost::property_tree::ptree(v.toHex())));
                }
                pt.push_back(std::make_pair("serviceIds", vals));
            }

            pt.add("revision", info._revision);
            pt.add("name", info._name);
            pt.add("description", info._description);

            if(!info._tags.empty())
            {
                boost::property_tree::ptree vals;
                for(const auto &v : info._tags)
                {
                    vals.push_back(std::make_pair("", boost::property_tree::ptree(v)));
                }
                pt.push_back(std::make_pair("tags", vals));
            }

            if(!info._requiredServiceIds.empty())
            {
                boost::property_tree::ptree vals;
                for(const auto &v : info._requiredServiceIds)
                {
                    vals.push_back(std::make_pair("", boost::property_tree::ptree(v.toHex())));
                }
                pt.push_back(std::make_pair("requiredServiceIds", vals));
            }

            if(!info._requiredModuleIds.empty())
            {
                boost::property_tree::ptree vals;
                for(const auto &v : info._requiredModuleIds)
                {
                    vals.push_back(std::make_pair("", boost::property_tree::ptree(v.toHex())));
                }
                pt.push_back(std::make_pair("requiredModuleIds", vals));
            }

            pt.add("mainBinary", boost::filesystem::path(mainBinaryFullPath).filename().string());
        }

        dlclose(mainBinaryHandle);

        std::stringstream ss;
        boost::property_tree::write_json(ss, pt);
        return ss.str();
    }

    Module::Module(Manager *site)
        : _manager{site}
        , _mainBinary{}
        , _state{ModuleState::null}
        , _place{}
        , _mainBinaryHandle{}
        , _entry{}
        , _info()
    {
    }

    Module::~Module()
    {
        assert(ModuleState::null == _state || ModuleState::attachError == _state);
    }

    const ModuleInfo &Module::getInfo() const
    {
        return _info;
    }

    ModuleState Module::getState() const
    {
        return _state;
    }

    std::error_code Module::attach(const ModulePlace &place)
    {
        if(ModuleState::null != _state)
        {
            return err_module::wrong_state;
        }

        _place = place;
        _state = ModuleState::attachError;

        boost::property_tree::ptree pt, npt;

        boost::filesystem::path path(_place.getDir());
        path /= "manifest.json";

        try
        {
            boost::property_tree::read_json(path.string(), pt);
        }
        catch(std::runtime_error &e)
        {
            LOGE("unable to parse module info: "<<e.what());
            return err_module::bad_info_file;
        }


        try
        {
            _info._provider = pt.get<std::string>("provider");
            if(!_info._id.fromHex(pt.get<std::string>("id")))
            {
               throw std::runtime_error("malformed id: "+pt.get<std::string>("id"));
            }

            _info._serviceIds.clear();
            for(auto &v: pt.get_child_optional("serviceIds").get_value_or(npt))
            {
                _info._serviceIds.push_back(couple::runtime::Iid{});
                if(!_info._serviceIds.back().fromHex(v.second.data()))
                {
                   throw std::runtime_error("malformed service id: "+v.second.data());
                }
            }

            _info._revision = pt.get<std::size_t>("revision");
            _info._name = pt.get<std::string>("name");
            _info._description = pt.get<std::string>("description");

            _info._tags.clear();
            for(auto &v: pt.get_child_optional("tags").get_value_or(npt))
            {
                _info._tags.emplace_back(v.second.data());
            }

            _info._requiredServiceIds.clear();
            for(auto &v: pt.get_child_optional("requiredServiceIds").get_value_or(npt))
            {
                _info._requiredServiceIds.push_back(couple::runtime::Iid{});
                if(!_info._requiredServiceIds.back().fromHex(v.second.data()))
                {
                   throw std::runtime_error("malformed required service id: "+v.second.data());
                }
            }

            _info._requiredModuleIds.clear();
            for(auto &v: pt.get_child_optional("requiredModuleIds").get_value_or(npt))
            {
                _info._requiredModuleIds.push_back(Mid{});
                if(!_info._requiredModuleIds.back().fromHex(v.second.data()))
                {
                   throw std::runtime_error("malformed required module id: "+v.second.data());
                }
            }

            _mainBinary = pt.get<std::string>("mainBinary");
        }
        catch(std::runtime_error &e)
        {
            LOGE("unable to interpret module info: "<<e.what());
            return err_module::bad_info_file;
        }

        _state = ModuleState::attached;

        return std::error_code{};
    }

    std::error_code Module::detach()
    {
        switch(_state)
        {
        case ModuleState::attached:
        case ModuleState::attachError:
        case ModuleState::loadError:
            break;

        case ModuleState::null:
        case ModuleState::loading:
        case ModuleState::loaded:
        case ModuleState::unloading:
        case ModuleState::starting:
        case ModuleState::started:
        case ModuleState::startError:
        case ModuleState::stopping:
        default:
            return err_module::wrong_state;
        }

        _info._provider.clear();
        //_id;
        _info._serviceIds.clear();

        _info._revision=0;
        _info._name.clear();
        _info._description.clear();
        _info._tags.clear();

        _info._requiredServiceIds.clear();
        _info._requiredModuleIds.clear();

        _state = ModuleState::null;
        _place.setDir(std::string{});

        _mainBinary.clear();

        return std::error_code{};
    }

    async::Future<std::error_code> Module::install(const ModulePlace &place)
    {
        (void)place;
        assert(0&&"not impl");
        return async::Future<std::error_code>(make_error_code(std::errc::address_not_available));
    }

    async::Future<std::error_code> Module::uninstall()
    {
        assert(0&&"not impl");
        return async::Future<std::error_code>(make_error_code(std::errc::address_not_available));
    }

    async::Future<std::error_code> Module::load()
    {
        switch(_state)
        {
        case ModuleState::attached:
        case ModuleState::loadError:
            break;

        case ModuleState::null:
        case ModuleState::attachError:
        case ModuleState::loading:
        case ModuleState::loaded:
        case ModuleState::unloading:
        case ModuleState::starting:
        case ModuleState::started:
        case ModuleState::startError:
        case ModuleState::stopping:
        default:
            return async::Future<std::error_code>(make_error_code(err_module::wrong_state));
        }

        _state = ModuleState::loading;

        return async::spawn([this] (async::Promise<std::error_code> p) mutable {

            assert(ModuleState::loading == _state);

            boost::filesystem::path mainBinaryPath{_place.getDir()};
            mainBinaryPath /= _mainBinary;

            assert(!_mainBinaryHandle);
            _mainBinaryHandle = dlopen(mainBinaryPath.string().c_str(), RTLD_NOW|RTLD_LOCAL|RTLD_NODELETE);

            if(!_mainBinaryHandle)
            {
                LOGE(dlerror());
                _state = ModuleState::loadError;
                p.resolveError(err_module::unable_load_binary);
                return;
            }

            assert(!_entry);

            void **ppe = static_cast<void **>(dlsym(_mainBinaryHandle, "dciModuleEntry"));
            if(!ppe)
            {
                LOGE("loading module \""<<_info._name<<"\": entry point is absent");
                _state = ModuleState::loadError;
                p.resolveError(err_module::unable_load_binary);
                return;
            }

            _entry = static_cast<ModuleEntry*>(*ppe);

            if(!_entry)
            {
                LOGE("loading module \""<<_info._name<<"\": entry point is damaged");
                _state = ModuleState::loadError;
                p.resolveError(err_module::unable_load_binary);
                return;
            }

            auto f = _entry->load(himpl::impl2Face<dci::site::ModulePlace>(_place));
            if(f.hasError())
            {
                LOGE("loading module \""<<_info._name<<"\": "<<f.error());

                _entry  = nullptr;

                assert(_mainBinaryHandle);
                dlclose(_mainBinaryHandle);
                _mainBinaryHandle = nullptr;

                _state = ModuleState::loadError;
                p.resolveError(f.detachError());
                return;
            }

            _state = ModuleState::loaded;
            p.resolveValue();
        });
    }

    async::Future<std::error_code> Module::unload()
    {
        switch(_state)
        {
        case ModuleState::loaded:
        case ModuleState::loadError:
            break;

        case ModuleState::null:
        case ModuleState::attached:
        case ModuleState::attachError:
        case ModuleState::loading:
        case ModuleState::unloading:
        case ModuleState::starting:
        case ModuleState::started:
        case ModuleState::startError:
        case ModuleState::stopping:
        default:
            return async::Future<std::error_code>(make_error_code(err_module::wrong_state));
        }

        _state = ModuleState::unloading;

        return async::spawn([this] (async::Promise<std::error_code> p) mutable {

            assert(ModuleState::unloading == _state);
            //assert(_entry);

            if(_entry)
            {
                auto f = _entry->unload(himpl::impl2Face<dci::site::ModulePlace>(_place));
                if(f.hasError())
                {
                    LOGE("unloading module \""<<_info._name<<"\": "<<f.error());
                    //ignore error
                }
            }

            _entry  = nullptr;


            assert(_mainBinaryHandle);
            dlclose(_mainBinaryHandle);
            _mainBinaryHandle = nullptr;

            _state = ModuleState::attached;
            p.resolveValue();
            return;
        });
    }

    async::Future<std::error_code> Module::start()
    {
        switch(_state)
        {
        case ModuleState::loaded:
            break;

        case ModuleState::null:
        case ModuleState::attached:
        case ModuleState::attachError:
        case ModuleState::loading:
        case ModuleState::loadError:
        case ModuleState::unloading:
        case ModuleState::starting:
        case ModuleState::started:
        case ModuleState::startError:
        case ModuleState::stopping:
        default:
            return async::Future<std::error_code>(make_error_code(err_module::wrong_state));
        }

        _state = ModuleState::starting;

        return async::spawn([this] (async::Promise<std::error_code> p) mutable {

            assert(ModuleState::starting == _state);
            assert(_entry);

            auto f = _entry->start(*himpl::impl2Face<dci::site::Manager>(_manager), himpl::impl2Face<dci::site::ModulePlace>(_place));
            if(f.hasError())
            {
                LOGE("starting module \""<<_info._name<<"\": "<<f.error());
                _state = ModuleState::startError;
                p.resolveError(f.detachError());
                return;
            }

            _state = ModuleState::started;
            p.resolveValue();
            return;
        });
    }

    async::Future<std::error_code> Module::stop()
    {
        switch(_state)
        {
        case ModuleState::started:
        case ModuleState::startError:
            break;

        case ModuleState::null:
        case ModuleState::attached:
        case ModuleState::attachError:
        case ModuleState::loading:
        case ModuleState::loadError:
        case ModuleState::loaded:
        case ModuleState::unloading:
        case ModuleState::starting:
        case ModuleState::stopping:
        default:
            return async::Future<std::error_code>(make_error_code(err_module::wrong_state));
        }

        _state = ModuleState::stopping;

        return async::spawn([this] (async::Promise<std::error_code> p) mutable {

            assert(ModuleState::stopping == _state);
            assert(_entry);

            auto f = _entry->stop(himpl::impl2Face<dci::site::ModulePlace>(_place));
            if(f.hasError())
            {
                LOGE("stopping module \""<<_info._name<<"\": "<<f.error());
                //ignore error
            }

            _state = ModuleState::loaded;
            p.resolveValue();
        });
    }

    ServiceFactory *Module::allocServiceFactory(const couple::runtime::Iid &iid)
    {
        assert(_entry);
        return _entry->allocServiceFactory(iid);
    }

    void Module::freeServiceFactory(const couple::runtime::Iid &iid, ServiceFactory *factory)
    {
        assert(_entry);
        return _entry->freeServiceFactory(iid, factory);
    }

}}}
