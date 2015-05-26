#pragma once

#include <system_error>
#include <map>
#include "module.hpp"
#include <dci/site/serviceFactory.hpp>

#include <dci/site/testHub.hpp>

namespace dci { namespace site { namespace impl
{
    enum class WorkState
    {
        stopped,
        starting,
        started,
        stopping,
    };

    class Manager
    {
    public:
        Manager();
        ~Manager();

        std::error_code run(int argc, char *argv[], testHub::Stage testStage);
        async::Future<std::error_code> stop();

    public:
        static std::string generateManifest(const std::string &mainBinaryFullPath);
        static int executeTest(int argc, char *argv[], testHub::Stage stage, site::Manager *manager);

        //outFuture is async::Future<std::error_code, ConcreteIface>
        std::error_code createService(void *outFuture, const couple::runtime::Iid &iid);

    private:
        std::error_code initializeModules();
        async::Future<std::error_code> loadModules();
        async::Future<std::error_code> startModules();
        async::Future<std::error_code> stopModules();
        async::Future<std::error_code> unloadModules();
        std::error_code deinitializeModules();

        template <class F>
        async::Future<std::error_code> massModulesOperation(const std::string &name, F operation);

    private:
        bool        _modulesInitialized;
        bool        _modulesLoaded;
        bool        _modulesStarted;
        WorkState   _workState;

    private:
        //TODO: состояние по проинсталированным модулям
        std::vector<ModulePtr> _modules;

    private:
        struct ServiceFactoryEntry
        {
            Module *            _module;
            ServiceFactory *    _factory;
        };

        using ServiceFactories = std::map<couple::runtime::Iid, ServiceFactoryEntry>;
        ServiceFactories _serviceFactories;
    };
}}}
