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

                map<uint32, net::Link> ifs = nh.links().detachValue<0>();

                auto printLink = [&](uint32 id)
                {
                    net::Link &i = ifs.find(id)->second;

                    LOGD("-------------------------------------");
                    LOGD("id:  "<<i.id().value<0>());
                    LOGD("name:  "<<i.name().value<0>());

                    std::string flags = "";
                    {
                        uint32_t v = i.flags().value<0>();
                        if(v & (uint32_t)net::Link::Flags::up) flags += " up";
                        if(v & (uint32_t)net::Link::Flags::running) flags += " running";
                        if(v & (uint32_t)net::Link::Flags::broadcast) flags += " broadcast";
                        if(v & (uint32_t)net::Link::Flags::loopback) flags += " loopback";
                        if(v & (uint32_t)net::Link::Flags::p2p) flags += " p2p";
                        if(v & (uint32_t)net::Link::Flags::multicast) flags += " multicast";

                    }
                    LOGD("flags: "<<flags<<std::dec);
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
                            <<"ip: "<<std::hex<<std::setfill('0')
                                <<std::setw(2)<<(unsigned)octets[0]<<""
                                <<std::setw(2)<<(unsigned)octets[1]<<"."
                                <<std::setw(2)<<(unsigned)octets[2]<<""
                                <<std::setw(2)<<(unsigned)octets[3]<<"."
                                <<std::setw(2)<<(unsigned)octets[4]<<""
                                <<std::setw(2)<<(unsigned)octets[5]<<"."
                                <<std::setw(2)<<(unsigned)octets[6]<<""
                                <<std::setw(2)<<(unsigned)octets[7]<<"."
                                <<std::setw(2)<<(unsigned)octets[8]<<""
                                <<std::setw(2)<<(unsigned)octets[9]<<"."
                                <<std::setw(2)<<(unsigned)octets[10]<<""
                                <<std::setw(2)<<(unsigned)octets[11]<<"."
                                <<std::setw(2)<<(unsigned)octets[12]<<""
                                <<std::setw(2)<<(unsigned)octets[13]<<"."
                                <<std::setw(2)<<(unsigned)octets[14]<<""
                                <<std::setw(2)<<(unsigned)octets[15]<<", "
                            <<std::dec
                            <<"prefixLength: "<<n.prefixLength<<", "
                            <<"scope: "<<(int)n.scope
                        );
                    }
                };

                auto printLinks = [&]()
                {
                    LOGD("=============================================================");
                    LOGD("links amount: "<<ifs.size());

                    for(auto &i : ifs)
                    {
                        printLink(i.first);
                    }

                };

                auto useLink = [&](uint32 id, ::net::Link &i, bool printAll)
                {
                    i.signal_removed().connect([&, id]()
                    {
                        ifs.erase(id);
                        printLinks();
                    });

                    i.signal_flagsChanged().connect([&]()
                    {
                        printLinks();
                    });
                    i.signal_ip4Changed().connect([&]()
                    {
                        printLinks();
                    });
                    i.signal_ip6Changed().connect([&]()
                    {
                        printLinks();
                    });
                    i.signal_mtuChanged().connect([&]()
                    {
                        printLinks();
                    });
                    i.signal_nameChanged().connect([&]()
                    {
                        printLinks();
                    });


                    if(printAll)
                    {
                        printLinks();
                    }
                };

                for(auto &i : ifs)
                {
                    useLink(i.first, i.second, false);
                }

                nh.signal_linkAdded().connect([&](uint32 id, ::net::Link &&i) {
                    useLink(id, i, true);
                    ifs.emplace(id, std::move(i));
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
