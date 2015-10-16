#include "manager.hpp"
#include <dci/site/error.hpp>
#include <dci/poll/functions.hpp>
#include <dci/async.hpp>
#include <dci/logger.hpp>
#include <dci/gtest.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <cassert>
#include <tuple>
#include <iostream>

#include <dlfcn.h>

namespace fs = boost::filesystem;

namespace dci { namespace site
{
    extern TestStage g_testStage;
    extern Manager *g_testManager;
}}

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

    std::error_code Manager::run(int argc, char *argv[], TestStage testStage)
    {
        switch(_workState)
        {
        case WorkState::stopped:
            break;

        case WorkState::starting:
            return err_general::starting_in_progress;

        case WorkState::started:
            return err_general::already_started;

        case WorkState::stopping:
            return err_general::stopping_in_progress;

        default:
            assert("unknown work state");
            abort();
        }

        {
            std::error_code ec = poll::initialize();
            if(ec)
            {
                return ec;
            }

        }

        std::error_code testec{};
        _workState = WorkState::starting;
        async::spawn([this, argc, argv, testStage, &testec](){

            if(TestStage::min == testStage)
            {
                if(Manager::executeTest(argc, argv, testStage, himpl::impl2Face<site::Manager>(this)))
                {
                    testec = make_error_code(err_general::test_failed);
                }

                _workState = WorkState::started;
                stop();
                return;
            }

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

            if(TestStage::mload == testStage)
            {
                if(Manager::executeTest(argc, argv, testStage, himpl::impl2Face<site::Manager>(this)))
                {
                    testec = make_error_code(err_general::test_failed);
                }
                _workState = WorkState::started;
                stop();
                return;
            }

            f = startModules();
            if(f.hasError())
            {
                LOGE("start modules: "<<f.error());
            }

            _workState = WorkState::started;

            if(TestStage::mstart == testStage)
            {
                if(Manager::executeTest(argc, argv, testStage, himpl::impl2Face<site::Manager>(this)))
                {
                    testec = make_error_code(err_general::test_failed);
                }
                stop();
                return;
            }
        });


        std::error_code ec = poll::run();
        if(ec)
        {
            LOGE("poll execute: "<<ec);
            stop().wait();
            poll::deinitialize();
            return ec;
        }

        ec = poll::deinitialize();
        if(ec)
        {
            LOGE("poll deinitialize: "<<ec);
            return ec;
        }

        if(testec)
        {
            return testec;
        }

        return std::error_code();
    }

    async::Future<std::error_code> Manager::stop()
    {
        switch(_workState)
        {
        case WorkState::started:
            break;

        case WorkState::stopped:
            return async::Future<std::error_code>(make_error_code(err_general::already_stopped));

        case WorkState::starting:
            return async::Future<std::error_code>(make_error_code(err_general::starting_in_progress));

        case WorkState::stopping:
            return async::Future<std::error_code>(make_error_code(err_general::stopping_in_progress));

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

            ec = poll::stop();
            if(ec)
            {
                LOGE("stop poll: "<<ec);
                hasErrors = true;
            }

            if(hasErrors)
            {
                p.resolveError(err_general::partial_failed);
            }
            else
            {
                p.resolveValue();
            }
        });
    }

    std::string Manager::generateManifest(const std::string &mainBinaryFullPath)
    {
        return Module::generateManifest(mainBinaryFullPath);
    }

    int Manager::executeTest(int argc, char *argv[], TestStage stage, site::Manager *manager)
    {
        const char *stageStr = nullptr;
        switch(stage)
        {
        case TestStage::noenv:
            stageStr = "noenv";
            break;
        case TestStage::min:
            stageStr = "min";
            break;
        case TestStage::mload:
            stageStr = "mload";
            break;
        case TestStage::mstart:
            stageStr = "mstart";
            break;
        case TestStage::null:
        default:
            assert(0);
            abort();
        }

        std::string hubModuleName = "libdci-gtest-hub-";
        hubModuleName += stageStr;
        hubModuleName += ".so";

        void *hubModule = dlopen(hubModuleName.c_str(), RTLD_NOW | RTLD_LOCAL);
        if(!hubModule)
        {
            LOGF("unable to load test hub module for stage "<<stageStr<<", "<<dlerror());
            abort();
            //return -1;
        }

        int(*hubEntryPoint)(int, char *[]) = reinterpret_cast<int(*)(int, char *[])>(dlsym(hubModule, "dciTestHubEntryPoint"));
        if(!hubEntryPoint)
        {
            dlclose(hubModule);
            LOGF("missing entry point in test hub module for stage "<<stageStr);
            abort();
            //return -1;
        }

        g_testStage = stage;
        g_testManager = manager;
        int res = hubEntryPoint(argc, argv);
        g_testStage = TestStage::null;
        g_testManager = nullptr;

        if(dlclose(hubModule))
        {
            LOGF("unable to unload test hub module for stage "<<stageStr<<", "<<dlerror());
            abort();
            //return -1;
        }

        return res;
    }

    std::error_code Manager::createService(void *outFuture, const couple::runtime::Iid &iid)
    {
        ServiceFactories::const_iterator iter = _serviceFactories.find(iid);
        if(_serviceFactories.end() == iter)
        {
            return err_general::not_found;
        }

        iter->second._factory->createService(outFuture);

        return std::error_code();
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
                return err_general::modules_directory_absent;
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
                        make_error_code(err_general::partial_failed) :
                        std::error_code{};
        }

        return std::error_code{};
    }

    async::Future<std::error_code> Manager::loadModules()
    {
        if(!_modulesLoaded)
        {
            _modulesLoaded = true;

            return massModulesOperation("load", [this](const ModulePtr &c)->async::Future<std::error_code> {
                auto f = c->load();
                if(f.hasError())
                {
                    return f;
                }


                for(const couple::runtime::Iid &iid : c->getInfo()._serviceIds)
                {
                    ServiceFactoryEntry sfe;
                    sfe._module = c.get();
                    sfe._factory = c->allocServiceFactory(iid);
                    if(!sfe._factory)
                    {
                        LOGE("unable to obtain service factory for "<<iid.toHex()<<", module "<<c->getInfo()._name);
                        continue;
                    }
                    _serviceFactories.insert(std::make_pair(iid, sfe));
                }

                return f;
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

            for(const ServiceFactories::value_type &f : _serviceFactories)
            {
                f.second._module->freeServiceFactory(f.first, f.second._factory);
            }
            _serviceFactories.clear();

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
                        make_error_code(err_general::partial_failed) :
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
                p.resolveError(err_general::partial_failed);
            }
            else
            {
                p.resolveValue();
            }
        });
    }


}}}
