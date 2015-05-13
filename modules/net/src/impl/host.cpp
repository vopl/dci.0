#include "host.hpp"
#include "interface.hpp"
#include "../handlers/host.hpp"
#include "../handlers/interface.hpp"

#include <dci/logger.hpp>
#include <dci/async.hpp>

#include <cassert>
#include <cstring>
#include <functional>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <ifaddrs.h>

namespace impl
{
    Host::Host()
    {
        update();
    }

    Host::~Host()
    {
        for(auto h : _handlers)
        {
            h->dropImpl();
        }
        _handlers.clear();
    }

    std::error_code Host::startup()
    {
        assert(!_instance);

        if(!_instance)
        {
            _instance = new Host;
        }

        return std::error_code();
    }

    std::error_code Host::shutdown()
    {
        assert(_instance);

        if(_instance)
        {
            Host *instance = _instance;
            _instance = nullptr;
            delete instance;
        }

        return std::error_code();
    }

    Host *Host::instance()
    {
        return _instance;
    }

    void Host::registerHandler(handlers::Host *handler)
    {
        _handlers.insert(handler);
    }

    void Host::unregisterHandler(handlers::Host *handler)
    {
        _handlers.erase(handler);
    }

    const Host::Interfaces &Host::interfaces() const
    {
        return _interfaces;
    }

    namespace
    {
        void updateInterfaceParams(::impl::Interface *i)
        {
            int fd = socket(AF_INET, SOCK_DGRAM, 0);

            struct ifreq ifr;
            ifr.ifr_addr.sa_family = AF_INET;
            memcpy(ifr.ifr_name, i->name().data(), i->name().size()+1);

            if(ioctl(fd, SIOCGIFFLAGS, (caddr_t)&ifr))
            {
                LOGE("ioctl SIOCGIFFLAGS: "<<strerror(errno));
                ifr.ifr_ifru.ifru_flags = 0;
            }
            i->setFlags(ifr.ifr_ifru.ifru_flags);

            if(ioctl(fd, SIOCGIFMTU, (caddr_t)&ifr))
            {
                LOGE("ioctl SIOCGIFMTU: "<<strerror(errno));
                ifr.ifr_ifru.ifru_mtu = 0;
            }
            i->setMtu(ifr.ifr_ifru.ifru_mtu);

            close(fd);
        }
    }

    void Host::update()
    {
        ifaddrs *all;
        if(getifaddrs(&all))
        {
            LOGE("getifaddrs: "<<strerror(errno));
            _interfaces.clear();
            return;
        }

        Interfaces newInterfaces;
        std::map<std::string, list< ip4::Net>> ip4Nets;
        std::map<std::string, list< ip6::Net>> ip6Nets;

        for(ifaddrs *one = all; one; one = one->ifa_next)
        {
            Interface *interfaceImpl{nullptr};
            std::string name = one->ifa_name;

            switch(one->ifa_addr->sa_family)
            {
            case AF_PACKET:
                continue;
            case AF_INET:
                {
                    ip4::Net a;

                    memcpy(&a.address.octets, &((sockaddr_in *)one->ifa_addr)->sin_addr.s_addr, 4);
                    memcpy(&a.netmask.octets, &((sockaddr_in *)one->ifa_netmask)->sin_addr.s_addr, 4);
                    memcpy(&a.broadcast.octets, &((sockaddr_in *)one->ifa_ifu.ifu_broadaddr)->sin_addr.s_addr, 4);
                    ip4Nets[name].emplace_back(a);
                }
                break;
            case AF_INET6:
                {
                    ip6::Net a;

                    memcpy(&a.address.octets, &((sockaddr_in6 *)one->ifa_addr)->sin6_addr.__in6_u.__u6_addr8, 16);
                    a.scopeId = ((sockaddr_in6 *)one->ifa_addr)->sin6_scope_id;

                    a.prefixLength = 0;
                    for(int i(0); i<16; i++)
                    {
                        uint8 v = ((sockaddr_in6 *)one->ifa_netmask)->sin6_addr.__in6_u.__u6_addr8[i];
                        while(v & 1)
                        {
                            a.prefixLength++;
                            v >>= 1;
                        }
                    }

                    ip6Nets[name].emplace_back(a);
                }
                break;
            default:
                assert(0);
            }

            Interfaces::iterator iter = _interfaces.find(name);
            if(_interfaces.end() == iter)
            {
                interfaceImpl = new Interface(name);
                newInterfaces.insert(std::make_pair(name, std::unique_ptr<Interface>(interfaceImpl)));
            }
        }

        freeifaddrs(all);

        for(const auto &i : newInterfaces)
        {
            updateInterfaceParams(i.second.get());
            i.second->setIp4Nets(ip4Nets[i.second->name()]);
            i.second->setIp6Nets(ip6Nets[i.second->name()]);
        }

        for(const auto &i : _interfaces)
        {
            updateInterfaceParams(i.second.get());
            i.second->setIp4Nets(ip4Nets[i.second->name()]);
            i.second->setIp6Nets(ip6Nets[i.second->name()]);
        }

        for(auto &i : newInterfaces)
        {
            for(handlers::Host *h : _handlers)
            {
                dci::async::spawn([&]()
                {
                    h->interfaceAdded(* new handlers::Interface(i.second.get()));
                });
            }

            _interfaces.emplace(std::move(i));
        }
    }

    Host *Host::_instance {nullptr};

}
