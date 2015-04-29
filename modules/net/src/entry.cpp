#include <dci/site/moduleEntry.hpp>
#include <dci/logger/logger.hpp>

#include "net.hpp"

struct Info
    : dci::site::ModuleInfo
{
    Info()
    {
        _provider = "dci";
        _id.fromHex("874ca4fbf31985170b2ce0efbb20a5a5");
        _serviceIds.push_back(net::Host::_iid);

        _revision = 1;
        _name = "net";
        _description = "simple network module";
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

    dci::async::Future<std::error_code> start(dci::site::Instance &site, const dci::site::ModulePlace &place) override
    {
        (void)site;
        (void)place;
        return dci::async::Future<std::error_code>();
    }

    dci::async::Future<std::error_code> stop(const dci::site::ModulePlace &place) override
    {
        (void)place;
        return dci::async::Future<std::error_code>();
    }

    dci::async::Future<std::error_code, dci::couple::runtime::Iface> getServiceInstance(const dci::couple::runtime::Iid &iid) override
    {
        (void)iid;
        assert(0);
    }
} entry;

extern "C"
{
    dci::site::ModuleEntry *dciModuleEntry = &entry;
}
