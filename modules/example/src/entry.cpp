#include <dci/site/moduleEntry.hpp>
#include <dci/async.hpp>
#include <dci/logger/logger.hpp>

#include <algorithm>
#include <iomanip>
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

                list<net::Link> ifs = nh.links().detachValue<0>();

                auto printLink = [&](net::Link &i)
                {
                    LOGD("-------------------------------------");
                    LOGD("name:  "<<i.name().value<0>());
                    LOGD("flags: 0x"<<std::hex<<i.flags().value<0>()<<std::dec);
                    LOGD("mtu:   "<<i.mtu().value<0>());

                    auto x4 = i.ip4().value<0>();
                    for(net::ip4::LinkAddress n : x4)
                    {
                        const auto &octets = n.address.octets;
                        const auto &mask = n.netmask.octets;
                        const auto &broadcast = n.broadcast.octets;
                        LOGD("ip4 net: "
                            <<"ip: "<<(unsigned)octets[0]<<"."<<(unsigned)octets[1]<<"."<<(unsigned)octets[2]<<"."<<(unsigned)octets[3]<<", "
                            <<"mask: "<<(unsigned)mask[0]<<"."<<(unsigned)mask[1]<<"."<<(unsigned)mask[2]<<"."<<(unsigned)mask[3]<<", "
                            <<"broadcast: "<<(unsigned)broadcast[0]<<"."<<(unsigned)broadcast[1]<<"."<<(unsigned)broadcast[2]<<"."<<(unsigned)broadcast[3]);
                    }

                    auto x6 = i.ip6().value<0>();
                    for(const net::ip6::LinkAddress &n : x6)
                    {
                        const auto &octets = n.address.octets;
                        LOGD("ip6 net: "
                            <<"ip: "<<std::hex<<std::setfill('0')<<std::setw(2)
                                <<(unsigned)octets[0]<<""
                                <<(unsigned)octets[1]<<"."
                                <<(unsigned)octets[2]<<""
                                <<(unsigned)octets[3]<<"."
                                <<(unsigned)octets[4]<<""
                                <<(unsigned)octets[5]<<"."
                                <<(unsigned)octets[6]<<""
                                <<(unsigned)octets[7]<<"."
                                <<(unsigned)octets[8]<<""
                                <<(unsigned)octets[9]<<"."
                                <<(unsigned)octets[10]<<""
                                <<(unsigned)octets[11]<<"."
                                <<(unsigned)octets[12]<<""
                                <<(unsigned)octets[13]<<"."
                                <<(unsigned)octets[14]<<""
                                <<(unsigned)octets[15]<<", "
                            <<std::dec
                            <<"prefixLength: "<<n.prefixLength<<", "
                            //<<"scopeId: "<<n.scopeId
                        );
                    }
                };

                auto printLinks = [&]()
                {
                    LOGD("links amount: "<<ifs.size());

                    for(net::Link &i : ifs)
                    {
                        printLink(i);
                    }

                };

                auto useLink = [&](::net::Link &i, bool printAll)
                {
                    LOGD("link added: "<<i.name().value<0>());

                    i.signal_removed().connect([&]()
                    {
                        LOGD("link removed: "<<i.name().value<0>());
                        std::remove_if(ifs.begin(), ifs.end(), [&](const net::Link &v)
                        {
                            return &v == &i;
                        });
                        printLinks();
                    });

                    i.signal_flagsChanged().connect([&]()
                    {
                        LOGD("link flags changed: "<<i.name().value<0>());
                        printLink(i);
                    });
                    i.signal_ip4Changed().connect([&]()
                    {
                        LOGD("link nets4 changed: "<<i.name().value<0>());
                        printLink(i);
                    });
                    i.signal_ip6Changed().connect([&]()
                    {
                        LOGD("link nets6 changed: "<<i.name().value<0>());
                        printLink(i);
                    });
                    i.signal_mtuChanged().connect([&]()
                    {
                        LOGD("link mtu changed: "<<i.name().value<0>());
                        printLink(i);
                    });
                    i.signal_nameChanged().connect([&]()
                    {
                        LOGD("link name changed: "<<i.name().value<0>());
                        printLink(i);
                    });


                    if(printAll)
                    {
                        printLinks();
                    }
                };

                for(net::Link &i : ifs)
                {
                    useLink(i, false);
                }

                nh.signal_linkAdded().connect([&](::net::Link &&i) {
                    ifs.emplace_back(std::move(i));
                    useLink(ifs.back(), true);
                });

                printLinks();
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
