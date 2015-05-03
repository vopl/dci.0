#include "manager.hpp"
#include <dci/site/error.hpp>
#include <dci/io/loop.hpp>
#include <dci/async/acquire.hpp>
#include <dci/async/functions.hpp>
#include <dci/logger/logger.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <cassert>
#include <tuple>
#include <iostream>

namespace fs = boost::filesystem;

namespace dci { namespace site { namespace impl
{
    Manager::Manager()
        : _modulesInitialized{false}
        , _modulesLoaded{false}
        , _modulesStarted{false}
        , _workState{WorkState::stopped}
    {
    }

    Manager::~Manager()
    {
    }

    std::error_code Manager::run()
    {
        switch(_workState)
        {
        case WorkState::stopped:
            break;

        case WorkState::starting:
            return make_error_code(error::general::starting_in_progress);

        case WorkState::started:
            return make_error_code(error::general::already_started);

        case WorkState::stopping:
            return make_error_code(error::general::stopping_in_progress);

        default:
            assert("unknown work state");
            abort();
        }

        _workState = WorkState::starting;
        async::spawn([this](){

            std::error_code ec = initializeModules();
            if(ec)
            {
                LOGE("initialize modules: "<<ec);
            }

            auto f = loadModules();
            if(f.hasError())
            {
                LOGE("load modules: "<<f.error());
            }

            f = startModules();
            if(f.hasError())
            {
                LOGE("start modules: "<<f.error());
            }

            _workState = WorkState::started;
        });

        return dci::io::loop::run();
    }

    async::Future<std::error_code> Manager::stop()
    {
        switch(_workState)
        {
        case WorkState::started:
            break;

        case WorkState::stopped:
            return async::Future<std::error_code>(make_error_code(error::general::already_stopped));

        case WorkState::starting:
            return async::Future<std::error_code>(make_error_code(error::general::starting_in_progress));

        case WorkState::stopping:
            return async::Future<std::error_code>(make_error_code(error::general::stopping_in_progress));

        default:
            assert("unknown work state");
            abort();
        }

        _workState = WorkState::stopping;
        return async::spawn([this](async::Promise<std::error_code> p) mutable{

            bool hasErrors = false;

            auto f = stopModules();
            if(f.hasError())
            {
                LOGE("stop modules: "<<f.error());
                hasErrors = true;
            }

            f = unloadModules();
            if(f.hasError())
            {
                LOGE("unload modules: "<<f.error());
                hasErrors = true;
            }

            std::error_code ec = deinitializeModules();
            if(ec)
            {
                LOGE("detach modules: "<<ec);
                hasErrors = true;
            }


            _workState = WorkState::stopped;

            ec = io::loop::stop();
            if(ec)
            {
                LOGE("stop io loop: "<<ec);
                hasErrors = true;
            }

            if(hasErrors)
            {
                p.resolveError(make_error_code(error::general::partial_failed));
            }
            else
            {
                p.resolveValue();
            }
        });
    }

    std::error_code Manager::initializeModules()
    {
        if(!_modulesInitialized)
        {
            _modulesInitialized = true;

            fs::path modulesDir = fs::current_path() / "share/dci-modules";

            if(!fs::exists(modulesDir))
            {
                LOGE("site initialization: modules direactory is absent");
                return make_error_code(error::general::modules_directory_absent);
            }

            bool hasErrors = false;

            fs::directory_iterator diter(modulesDir);
            fs::directory_iterator dend;
            for(; diter!=dend; ++diter)
            {
                if(fs::is_directory(*diter))
                {
                    ModulePtr c{new Module{this}};
                    std::error_code ec = c->attach(diter->path().string());
                    if(ec)
                    {
                        LOGE("unable attach module in "<<diter->path().string()<<": "<<ec);
                        hasErrors = true;
                    }
                    else
                    {
                        _modules.emplace_back(std::move(c));
                    }
                }
            }

            return hasErrors ?
                        make_error_code(error::general::partial_failed) :
                        std::error_code{};
        }

        return std::error_code{};
    }

    async::Future<std::error_code> Manager::loadModules()
    {
        if(!_modulesLoaded)
        {
            _modulesLoaded = true;

            return massModulesOperation("load", [](const ModulePtr &c)->async::Future<std::error_code> {
                return c->load();
            });
        }

        return async::Future<std::error_code>();
    }

    async::Future<std::error_code> Manager::startModules()
    {
        if(!_modulesStarted)
        {
            _modulesStarted = true;

            return massModulesOperation("start", [](const ModulePtr &c)->async::Future<std::error_code> {
                return c->start();
            });
        }

        return async::Future<std::error_code>();
    }

    async::Future<std::error_code> Manager::stopModules()
    {
        if(_modulesStarted)
        {
            _modulesStarted = false;

            return massModulesOperation("stop", [](const ModulePtr &c)->async::Future<std::error_code> {
                return c->stop();
            });
        }

        return async::Future<std::error_code>();
    }

    async::Future<std::error_code> Manager::unloadModules()
    {
        if(_modulesLoaded)
        {
            _modulesLoaded = false;

            return massModulesOperation("unload", [](const ModulePtr &c)->async::Future<std::error_code> {
                return c->unload();
            });
        }

        return async::Future<std::error_code>();
    }

    std::error_code Manager::deinitializeModules()
    {
        if(_modulesInitialized)
        {
            _modulesInitialized = false;

            bool hasErrors = false;

            for(const auto &c : _modules)
            {
                std::error_code ec = c->detach();
                if(ec)
                {
                    LOGE("unable detach module \""<<c->getInfo()._name<<"\": "<<ec);
                    hasErrors = true;
                }
            }

            _modules.clear();

            return hasErrors ?
                        make_error_code(error::general::partial_failed) :
                        std::error_code{};
        }

        return std::error_code{};
    }

    template <class F>
    async::Future<std::error_code> Manager::massModulesOperation(const std::string &name, F operation)
    {
        return async::spawn([this, name, operation](async::Promise<std::error_code> p) mutable {

            bool hasErrors = false;

            std::vector<std::tuple<Module*, async::Future<std::error_code>>> results;
            results.reserve(_modules.size());
            for(const auto &c : _modules)
            {
                results.emplace_back(std::make_tuple(c.get(), operation(c)));
            }

            for(auto &r : results)
            {
                if(std::get<1>(r).hasError())
                {
                    LOGE(name<<" module \""<<std::get<0>(r)->getInfo()._name<<"\": "<<std::get<1>(r).error());
                    hasErrors = true;
                }
            }

            if(hasErrors)
            {
                p.resolveError(make_error_code(error::general::partial_failed));
            }
            else
            {
                p.resolveValue();
            }
        });
    }


}}}
