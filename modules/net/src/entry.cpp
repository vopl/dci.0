#include <dci/site/moduleEntryFunctions.hpp>
#include <dci/logger/logger.hpp>

#include "net.hpp"

struct Entry
    : dci::site::ModuleEntry
{
    Entry()
    {
    }

    ~Entry()
    {
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

    dci::async::Future<std::error_code> start(const dci::site::ModulePlace &place) override
    {
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
