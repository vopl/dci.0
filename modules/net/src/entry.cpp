#include <dci/site/moduleEntry.hpp>
#include <dci/mm/newDelete.hpp>
#include <dci/logger/logger.hpp>

#include "net.hpp"
#include "netHandlerSkel.hpp"

using namespace dci::couple::runtime;
using namespace dci::site;
using namespace net;

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
        return dci::async::Future<std::error_code>();
    }

    dci::async::Future<std::error_code> unload(const ModulePlace &place) override
    {
        (void)place;
        return dci::async::Future<std::error_code>();
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


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    struct NetHostHandler
        : hs::net::Host<NetHostHandler>
    {
        Future<list<Interface>> interfaces()
        {
            //do impl
            //assert(0);

            list<Interface> l;

            return Future<list<Interface>>(std::move(l));
        }

        Future< ip4::stream::Host> ip4StreamHost()
        {
            assert(0);
        }

        Future< ip4::datagram::Host> ip4DatagramHost()
        {
            assert(0);
        }

        Future< ip6::stream::Host> ip6StreamHost()
        {
            assert(0);
        }

        Future< ip6::datagram::Host> ip6DatagramHost()
        {
            assert(0);
        }

        Future< local::stream::Host> localStreamHost()
        {
            assert(0);
        }


    };

    struct NetHostFactory
        : ServiceFactory
    {
        void createService(void *outFuture) override
        {
            dci::async::Future<std::error_code, Host> *res = static_cast<dci::async::Future<std::error_code, Host> *>(outFuture);
            *res = dci::async::Future<std::error_code, Host>(Host(* new NetHostHandler));
        }
    };

    ServiceFactory *allocServiceFactory(const Iid &iid) override
    {
        assert(iid == info._serviceIds[0]);
        return new NetHostFactory();
    }

    void freeServiceFactory(const Iid &iid, ServiceFactory *factory) override
    {
        assert(iid == info._serviceIds[0]);
        delete factory;
    }

} entry;

extern "C"
{
    ModuleEntry *dciModuleEntry = &entry;
}
