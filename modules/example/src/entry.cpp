#include <dci/site/moduleEntry.hpp>
#include <dci/logger/logger.hpp>

#include "net.hpp"










struct Info
    : dci::site::ModuleInfo
{
    Info()
    {
        //using namespace ns;
        {
            std::error_code code(net::general::no_interface);

            bool b1 = code == net::general::no_interface;
            (void)b1;
            bool b2 = net::general::channel_closed == code;
            (void)b2;

            int k = 220;
            (void)k;

            LOGD(code);
        }




        _provider = "dci";
        _id.fromHex("8785599b2e858e7d0f55a888b87127ed");
        //_serviceIds;

        _revision = 1;
        _name = "example";
        _description = "example or victim module";
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
        (void)place;

        dci::async::Future<std::error_code, net::Host> netHost = manager.createService<net::Host>();

        netHost.wait();
        if(netHost.hasError())
        {
            LOGD(netHost.error());
        }
        else
        {
            LOGD("service acuired");
            net::Host nh = netHost.detachValue<0>();

            nh.signal_interfaceAdded().connect([](::net::Interface &&i) {

                (void)i;

                int k = 220;
                (void)k;
            });


            auto ifs = nh.interfaces().detachValue<0>();

            std::size_t x = ifs.size();
            (void)x;

            int k = 220;
            (void)k;
        }

        return dci::async::Future<std::error_code>();
    }

    dci::async::Future<std::error_code> stop(const dci::site::ModulePlace &place) override
    {
        (void)place;
        return dci::async::Future<std::error_code>();
    }

    dci::site::ServiceFactory *allocServiceFactory(const dci::couple::runtime::Iid &iid) override
    {
        (void)iid;
        assert(0);
    }

    void freeServiceFactory(const dci::couple::runtime::Iid &iid, dci::site::ServiceFactory *factory) override
    {
        (void)iid;
        (void)factory;
        assert(0);
    }
} entry;

extern "C"
{
    dci::site::ModuleEntry *dciModuleEntry = &entry;
}
