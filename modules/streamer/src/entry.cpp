#include <dci/site/moduleEntry.hpp>
#include <dci/async.hpp>
#include <dci/logger.hpp>


#include "handlers/hub.hpp"

using namespace dci::couple::runtime;
using namespace dci::site;
using namespace streamer;



struct Info
    : dci::site::ModuleInfo
{
    Info()
    {
        _provider = "dci";
        _id.fromHex("23bd7add4fc9c603b42bb20a41fe80a0");
        _serviceIds.push_back(Hub::_iid);

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
        assert(iid == info._serviceIds[0]);
        return new handlers::HubHandlerFactory;
    }

    void freeServiceFactory(const dci::couple::runtime::Iid &iid, dci::site::ServiceFactory *factory) override
    {
        assert(iid == info._serviceIds[0]);
        delete factory;
    }

} entry;

extern "C"
{
    DCI_API_EXPORT dci::site::ModuleEntry *dciModuleEntry = &entry;
}