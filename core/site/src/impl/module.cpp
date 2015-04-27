#include "module.hpp"
#include <error.hpp>
#include <dci/logger/logger.hpp>
#include <dci/async/functions.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>

#include <dlfcn.h>

namespace dci { namespace site { namespace impl
{
    Module::Module()
        : _provider{}
        , _id{}
        , _serviceIds{}

        , _revision{}
        , _name{}
        , _description{}
        , _tags{}

        , _requiredServiceIds{}
        , _requiredModuleIds{}

        , _mainBinary{}

        , _state{ModuleState::null}
        , _place{}

        , _mainBinaryHandle{}
        , _entry{}
    {
    }

    Module::~Module()
    {
        assert(ModuleState::null == _state || ModuleState::attachError == _state);
    }

    const std::string &Module::getProvider() const
    {
        return _provider;
    }

    const Mid &Module::getId() const
    {
        return _id;
    }

    const std::vector<couple::runtime::Iid> &Module::getServieceIds() const
    {
        return _serviceIds;
    }

    std::size_t Module::getRevision() const
    {
        return _revision;
    }

    const std::string &Module::getName() const
    {
        return _name;
    }

    const std::string &Module::getDescription() const
    {
        return _description;
    }

    const std::vector<std::string> &Module::getTags() const
    {
        return _tags;
    }

    const std::vector<couple::runtime::Iid> &Module::getRequiredServiceIds() const
    {
        return _requiredServiceIds;
    }

    const std::vector<Mid> &Module::getRequiredModuleIds() const
    {
        return _requiredModuleIds;
    }

    ModuleState Module::getState() const
    {
        return _state;
    }

    std::error_code Module::attach(const ModulePlace &place)
    {
        if(ModuleState::null != _state)
        {
            return make_error_code(error::module::wrong_state);
        }

        _place = place;
        _state = ModuleState::attachError;

        boost::property_tree::ptree pt;

        boost::filesystem::path path(_place.getDir());
        path /= "index.json";

        try
        {
            boost::property_tree::read_json(path.string(), pt);
        }
        catch(std::runtime_error &e)
        {
            LOGE("unable to parse module info: "<<e.what());
            return make_error_code(error::module::bad_info_file);
        }


        try
        {
            _provider = pt.get<std::string>("provider");
            if(!_id.fromHex(pt.get<std::string>("id")))
            {
               throw std::runtime_error("malformed id: "+pt.get<std::string>("id"));
            }

            _serviceIds.clear();
            for(auto &v: pt.get_child("serviceIds"))
            {
                _serviceIds.push_back(couple::runtime::Iid{});
                if(!_serviceIds.back().fromHex(v.second.data()))
                {
                   throw std::runtime_error("malformed service id: "+v.second.data());
                }
            }

            _revision = pt.get<std::size_t>("revision");
            _name = pt.get<std::string>("name");
            _description = pt.get<std::string>("description");

            _tags.clear();
            for(auto &v: pt.get_child("tags"))
            {
                _tags.emplace_back(v.second.data());
            }

            _requiredServiceIds.clear();
            for(auto &v: pt.get_child("requiredServiceIds"))
            {
                _requiredServiceIds.push_back(couple::runtime::Iid{});
                if(!_requiredServiceIds.back().fromHex(v.second.data()))
                {
                   throw std::runtime_error("malformed required service id: "+v.second.data());
                }
            }

            _requiredModuleIds.clear();
            for(auto &v: pt.get_child("requiredModuleIds"))
            {
                _requiredModuleIds.push_back(Mid{});
                if(!_requiredModuleIds.back().fromHex(v.second.data()))
                {
                   throw std::runtime_error("malformed required module id: "+v.second.data());
                }
            }

            _mainBinary = pt.get<std::string>("mainBinary");
        }
        catch(std::runtime_error &e)
        {
            LOGE("unable to interpret module info: "<<e.what());
            return make_error_code(error::module::bad_info_file);
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

        default:
            return make_error_code(error::module::wrong_state);
        }

        _provider.clear();
        //_id;
        _serviceIds.clear();

        _revision=0;
        _name.clear();
        _description.clear();
        _tags.clear();

        _requiredServiceIds.clear();
        _requiredModuleIds.clear();

        _state = ModuleState::null;
        _place.setDir(std::string{});

        _mainBinary.clear();

        return std::error_code{};
    }

    async::Future<std::error_code> Module::install(const ModulePlace &place)
    {
        (void)place;
        assert(!"not impl");
    }

    async::Future<std::error_code> Module::uninstall()
    {
        assert(!"not impl");
    }

    async::Future<std::error_code> Module::load()
    {
        switch(_state)
        {
        case ModuleState::attached:
        case ModuleState::loadError:
            break;

        default:
            return async::Future<std::error_code>(make_error_code(error::module::wrong_state));
        }

        _state = ModuleState::loading;

        return async::spawn([this] (async::Promise<std::error_code> p) mutable {

            assert(ModuleState::loading == _state);

            boost::filesystem::path mainBinaryPath{_place.getDir()};
            mainBinaryPath /= _mainBinary;

            assert(!_mainBinaryHandle);
            _mainBinaryHandle = dlopen(mainBinaryPath.string().c_str(), RTLD_LAZY|RTLD_LOCAL);

            if(!_mainBinaryHandle)
            {
                LOGE(dlerror());
                _state = ModuleState::loadError;
                p.resolveError(make_error_code(error::module::unable_load_binary));
                return;
            }

            assert(!_entry);

            void **ppe = (void **)dlsym(_mainBinaryHandle, "dciModuleEntry");
            if(!ppe)
            {
                LOGE("loading module \""<<_name<<"\": entry point is absent");
                _state = ModuleState::loadError;
                p.resolveError(make_error_code(error::module::unable_load_binary));
                return;
            }

             _entry = static_cast<ModuleEntry*>(*ppe);

             if(!_entry)
             {
                 LOGE("loading module \""<<_name<<"\": entry point is damaged");
                 _state = ModuleState::loadError;
                 p.resolveError(make_error_code(error::module::unable_load_binary));
                 return;
             }

             auto f = _entry->load(himpl::impl2Face<dci::site::ModulePlace>(_place));
             if(f.hasError())
             {
                 LOGE("loading module \""<<_name<<"\": "<<f.error());

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

        default:
            return async::Future<std::error_code>(make_error_code(error::module::wrong_state));
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
                    LOGE("unloading module \""<<_name<<"\": "<<f.error());
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

        default:
            return async::Future<std::error_code>(make_error_code(error::module::wrong_state));
        }

        _state = ModuleState::starting;

        return async::spawn([this] (async::Promise<std::error_code> p) mutable {

            assert(ModuleState::starting == _state);
            assert(_entry);

            auto f = _entry->start(himpl::impl2Face<dci::site::ModulePlace>(_place));
            if(f.hasError())
            {
                LOGE("starting module \""<<_name<<"\": "<<f.error());
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

        default:
            return async::Future<std::error_code>(make_error_code(error::module::wrong_state));
        }

        _state = ModuleState::stopping;

        return async::spawn([this] (async::Promise<std::error_code> p) mutable {

            assert(ModuleState::stopping == _state);
            assert(_entry);

            auto f = _entry->stop(himpl::impl2Face<dci::site::ModulePlace>(_place));
            if(f.hasError())
            {
                LOGE("stopping module \""<<_name<<"\": "<<f.error());
                //ignore error
            }

            _state = ModuleState::loaded;
            p.resolveValue();
        });
    }

    async::Future<std::error_code, couple::runtime::Iface> Module::getServiceInstance(const couple::runtime::Iid &iid)
    {
        (void)iid;
        assert(0);
    }

}}}
