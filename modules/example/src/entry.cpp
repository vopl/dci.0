#include <dci/site/moduleEntry.hpp>
#include <dci/async.hpp>
#include <dci/logger/logger.hpp>

#include <algorithm>
#include "net.hpp"










struct Info
    : dci::site::ModuleInfo
{
    Info()
    {
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

        _stopEvent.reset();

        dci::async::spawn([&]()
        {
            using namespace ::dci::couple::runtime;

            dci::async::Future<std::error_code, net::Host> netHost = manager.createService<net::Host>();

            if(netHost.hasError())
            {
                LOGD(netHost.error());
            }
            else
            {
                LOGD("start net watching");
                net::Host nh = netHost.detachValue<0>();

                list<net::Interface> ifs = nh.interfaces().detachValue<0>();

                auto printInterface = [&](net::Interface &i)
                {
                    LOGD("-------------------------------------");
                    LOGD("name:  "<<i.name().value<0>());
                    LOGD("flags: 0x"<<std::hex<<i.flags().value<0>()<<std::dec);
                    LOGD("mtu:   "<<i.mtu().value<0>());

                    auto x4 = i.ip4Nets().value<0>();
                    for(net::ip4::Net n : x4)
                    {
                        const auto &octets = n.address.octets;
                        const auto &mask = n.netmask.octets;
                        const auto &broadcast = n.broadcast.octets;
                        LOGD("ip4 net: "
                            <<"ip: "<<(unsigned)octets[0]<<"."<<(unsigned)octets[1]<<"."<<(unsigned)octets[2]<<"."<<(unsigned)octets[3]<<", "
                            <<"mask: "<<(unsigned)mask[0]<<"."<<(unsigned)mask[1]<<"."<<(unsigned)mask[2]<<"."<<(unsigned)mask[3]<<", "
                            <<"broadcast: "<<(unsigned)broadcast[0]<<"."<<(unsigned)broadcast[1]<<"."<<(unsigned)broadcast[2]<<"."<<(unsigned)broadcast[3]);
                    }

                    auto x6 = i.ip6Nets().value<0>();
                    for(const net::ip6::Net &n : x6)
                    {
                        const auto &octets = n.address.octets;
                        LOGD("ip6 net: "
                            <<"ip: "<<(unsigned)octets[0]<<"."<<(unsigned)octets[1]<<"."<<(unsigned)octets[2]<<"."<<(unsigned)octets[3]<<"..., "
                            <<"prefixLength: "<<n.prefixLength<<", "
                            <<"scopeId: "<<n.scopeId);
                    }
                };

                auto printInterfaces = [&]()
                {
                    LOGD("interfaces amount: "<<ifs.size());

                    for(net::Interface &i : ifs)
                    {
                        printInterface(i);
                    }

                };

                auto useInterface = [&](::net::Interface &i, bool printAll)
                {
                    LOGD("interface added: "<<i.name().value<0>());

                    i.signal_removed().connect([&]()
                    {
                        LOGD("interface removed: "<<i.name().value<0>());
                        std::remove_if(ifs.begin(), ifs.end(), [&](const net::Interface &v)
                        {
                            return &v == &i;
                        });
                        printInterfaces();
                    });

                    i.signal_flagsChanged().connect([&]()
                    {
                        LOGD("interface flags changed: "<<i.name().value<0>());
                        printInterface(i);
                    });
                    i.signal_ip4NetsChanged().connect([&]()
                    {
                        LOGD("interface nets4 changed: "<<i.name().value<0>());
                        printInterface(i);
                    });
                    i.signal_ip6NetsChanged().connect([&]()
                    {
                        LOGD("interface nets6 changed: "<<i.name().value<0>());
                        printInterface(i);
                    });
                    i.signal_mtuChanged().connect([&]()
                    {
                        LOGD("interface mtu changed: "<<i.name().value<0>());
                        printInterface(i);
                    });
                    i.signal_nameChanged().connect([&]()
                    {
                        LOGD("interface name changed: "<<i.name().value<0>());
                        printInterface(i);
                    });


                    if(printAll)
                    {
                        printInterfaces();
                    }
                };

                for(net::Interface &i : ifs)
                {
                    useInterface(i, false);
                }

                nh.signal_interfaceAdded().connect([&](::net::Interface &&i) {
                    ifs.emplace_back(std::move(i));
                    useInterface(ifs.back(), true);
                });

                printInterfaces();
                _stopEvent.acquire();
                LOGD("stop net watching");
            }
        });

        return dci::async::Future<std::error_code>();
    }

    dci::async::Future<std::error_code> stop(const dci::site::ModulePlace &place) override
    {
        (void)place;

        _stopEvent.set();

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

private:
    dci::async::Event _stopEvent;
} entry;

extern "C"
{
    dci::site::ModuleEntry *dciModuleEntry = &entry;
}
