#include <dci/site/moduleEntry.hpp>
#include <dci/logger/logger.hpp>

#include "handlers/host.hpp"
#include "impl/host.hpp"

using namespace dci::couple::runtime;
using namespace dci::site;
using namespace net;

namespace
{

    struct Info
        : ModuleInfo
    {
        Info()
        {
            _provider = "dci";
            _id.fromHex("874ca4fbf31985170b2ce0efbb20a5a5");
            _serviceIds.push_back(Host::_iid);

            _revision = 1;
            _name = "net";
            _description = "simple network module";
            //_tags;

            //_requiredServiceIds;
            //_requiredModuleIds;
        }

    } info;


    struct Entry
        : ModuleEntry
    {
        Entry()
        {
        }

        ~Entry()
        {
        }

        const ModuleInfo &getInfo() override
        {
            return info;
        }

        dci::async::Future<std::error_code> install(const ModulePlace &place) override
        {
            (void)place;
            return dci::async::Future<std::error_code>();
        }

        dci::async::Future<std::error_code> uninstall(const ModulePlace &place) override
        {
            (void)place;
            return dci::async::Future<std::error_code>();
        }

        dci::async::Future<std::error_code> load(const ModulePlace &place) override
        {
            (void)place;

            return ::impl::Host::startup();
        }

        dci::async::Future<std::error_code> unload(const ModulePlace &place) override
        {
            (void)place;

            return ::impl::Host::shutdown();
        }

        dci::async::Future<std::error_code> start(Manager &manager, const ModulePlace &place) override
        {
            (void)manager;
            (void)place;
            return dci::async::Future<std::error_code>();
        }

        dci::async::Future<std::error_code> stop(const ModulePlace &place) override
        {
            (void)place;
            return dci::async::Future<std::error_code>();
        }

        ServiceFactory *allocServiceFactory(const Iid &iid) override
        {
            assert(iid == info._serviceIds[0]);
            (void)iid;
            return new handlers::HostHandlerFactory;
        }

        void freeServiceFactory(const Iid &iid, ServiceFactory *factory) override
        {
            assert(iid == info._serviceIds[0]);
            (void)iid;
            delete factory;
        }

    } entry;
}

extern "C"
{
    extern ModuleEntry *dciModuleEntry;
    DCI_API_EXPORT ModuleEntry *dciModuleEntry = &entry;
}
