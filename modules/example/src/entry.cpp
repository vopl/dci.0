#include <dci/site/moduleEntryFunctions.hpp>
#include <dci/logger/logger.hpp>

#include "talk.idl.hpp"

struct Entry
    : dci::site::ModuleEntry
{
    Entry()
    {
        talk::Peer p;
        talk::PeerOpposite op(p);


        op.setId().connect([](dci::couple::runtime::call::Value<talk::Peer::Id> &&id)->dci::async::Future<dci::couple::runtime::call::Error>{

            return dci::async::Future<dci::couple::runtime::call::Error>();
        });

        op.setStatus().connect([](const talk::Peer::Status &s){

            return dci::async::Future<dci::couple::runtime::call::Error>();
        });


        talk::Peer::Id id;

        dci::async::Future<dci::couple::runtime::call::Error> f = p.setId(std::move(id));

        if(f.hasError())
        {
            LOGT(f.error());
        }

        f = p.setStatus(talk::Peer::Status::busy);

        if(f.hasError())
        {
            LOGT(f.error());
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
