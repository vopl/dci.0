#include <dci/site/moduleEntry.hpp>
#include <dci/async.hpp>
#include <dci/logger.hpp>


#include "handlers/channelHub.hpp"
#include "handlers/serviceHub.hpp"
#include "handlers/amortisseur.hpp"

using namespace dci::couple::runtime;
using namespace dci::site;
using namespace streamer;


namespace
{

    struct Info
        : dci::site::ModuleInfo
    {
        Info()
        {
            _provider = "dci";
            _id.fromHex("23bd7add4fc9c603b42bb20a41fe80a0");
            _serviceIds.push_back(ChannelHub::_iid);
            _serviceIds.push_back(ServiceHub::_iid);
            _serviceIds.push_back(Amortisseur::_iid);

            _revision = 1;
            _name = "streamer";
            _description = "streamer: channel and hub";
            //_tags;

            //_requiredServiceIds;
            //_requiredModuleIds;
        }

    } info;

    struct Entry
        : dci::site::ModuleEntry
    {
        Entry()
        {
        }

        ~Entry()
        {
        }

        const dci::site::ModuleInfo &getInfo() override
        {
            return info;
        }

        dci::async::Future<std::error_code> install(const dci::site::ModulePlace &place) override
        {
            (void)place;
            return dci::async::Future<std::error_code>();
        }

        dci::async::Future<std::error_code> uninstall(const dci::site::ModulePlace &place) override
        {
            (void)place;
            return dci::async::Future<std::error_code>();
        }

        dci::async::Future<std::error_code> load(const dci::site::ModulePlace &place) override
        {
            (void)place;
            return dci::async::Future<std::error_code>();
        }

        dci::async::Future<std::error_code> unload(const dci::site::ModulePlace &place) override
        {
            (void)place;
            return dci::async::Future<std::error_code>();
        }

        dci::async::Future<std::error_code> start(dci::site::Manager &manager, const dci::site::ModulePlace &place) override
        {
            (void)manager;
            (void)place;

            return dci::async::Future<std::error_code>();
        }

        dci::async::Future<std::error_code> stop(const dci::site::ModulePlace &place) override
        {
            (void)place;

            return dci::async::Future<std::error_code>();
        }

        dci::site::ServiceFactory *allocServiceFactory(const dci::couple::runtime::Iid &iid) override
        {
            if(ChannelHub::_iid == iid)
            {
                return new handlers::ChannelHubHandlerFactory;
            }
            if(ServiceHub::_iid == iid)
            {
                return new handlers::ServiceHubHandlerFactory;
            }
            if(Amortisseur::_iid == iid)
            {
                return new handlers::AmortisseurHandlerFactory;
            }

            return nullptr;
        }

        void freeServiceFactory(const dci::couple::runtime::Iid &iid, dci::site::ServiceFactory *factory) override
        {
            if(ChannelHub::_iid == iid)
            {
                delete factory;
                return;
            }
            if(ServiceHub::_iid == iid)
            {
                delete factory;
                return;
            }
            if(Amortisseur::_iid == iid)
            {
                delete factory;
                return;
            }
        }

    } entry;
}

extern "C"
{
    extern dci::site::ModuleEntry *dciModuleEntry;
    DCI_API_EXPORT dci::site::ModuleEntry *dciModuleEntry = &entry;
}
