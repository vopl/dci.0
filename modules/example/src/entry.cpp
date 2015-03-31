#include <dci/site/moduleEntryFunctions.hpp>
#include "customIfaceHandler.hpp"
#include <dci/logger/logger.hpp>

struct Entry
    : dci::site::ModuleEntry
{
    Entry()
    {
        CustomIfaceHandler h;
        dci::async::Future<call::Error, int> f = h.f.in1();

        if(f.hasError())
        {
            LOGT(f.error());
        }
        else
        {
            LOGT(f.value<0>());
        }
    }

    ~Entry()
    {
    }

    dci::async::Future<std::error_code> install(const dci::site::ModulePlace &place) override
    {
        return dci::async::Future<std::error_code>();
    }

    dci::async::Future<std::error_code> uninstall(const dci::site::ModulePlace &place) override
    {
        return dci::async::Future<std::error_code>();
    }

    dci::async::Future<std::error_code> load(const dci::site::ModulePlace &place) override
    {
        return dci::async::Future<std::error_code>();
    }

    dci::async::Future<std::error_code> unload(const dci::site::ModulePlace &place) override
    {
        return dci::async::Future<std::error_code>();
    }

    dci::async::Future<std::error_code> start(const dci::site::ModulePlace &place) override
    {
        return dci::async::Future<std::error_code>();
    }

    dci::async::Future<std::error_code> stop(const dci::site::ModulePlace &place) override
    {
        return dci::async::Future<std::error_code>();
    }

    dci::async::Future<std::error_code, dci::couple::runtime::Iface> getServiceInstance(const dci::couple::runtime::Iid &iid) override
    {
        assert(0);
    }
} entry;

extern "C"
{
    dci::site::ModuleEntry *dciModuleEntry = &entry;
}
