#include "engineRtnetlink.hpp"
#include "host.hpp"
#include "link.hpp"
#include <dci/async.hpp>
#include <dci/logger.hpp>

#include <linux/rtnetlink.h>
#include <netinet/in.h>
#include <net/if.h>
#include <cstring>

namespace impl
{
    Rtnetlink::Rtnetlink(Host *host)
        : Engine(host)
        , _address()
        , _msgiov()
        , _msg()
    {
        dci::async::spawn(&Rtnetlink::worker, this);
        _initialized.acquire();
    }

    Rtnetlink::~Rtnetlink()
    {
        _stop.set();
        _stopped.acquire();
    }

    bool Rtnetlink::valid() const
    {
        return _sock && _sock->valid();
    }

    bool Rtnetlink::startup()
    {
        //socket
        assert(!_sock);
        _sock.reset(new dci::poller::Descriptor(socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE)));
        if(!_sock->valid())
        {
            _sock.reset();
            LOGE("rtnetlink invalid socket");
            return false;
        }

        //bind
        _address.nl_family = AF_NETLINK;
        _address.nl_pid = getpid();
        _address.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR | RTMGRP_IPV6_IFADDR;
        if(0 != bind(*_sock, (sockaddr*)&_address, sizeof(_address)))
        {
            LOGE("rtnetlink bind: "<<strerror(errno));
            _sock.reset();
            return false;
        }

        //message area
        assert(!_msgiov.iov_base);
        _msgiov.iov_len = 8192;
        _msgiov.iov_base = malloc(_msgiov.iov_len);
        if(!_msgiov.iov_base)
        {
            LOGE("rtnetlink malloc null");
            _sock.reset();
            return false;
        }

//        _msg.msg_name = &_address;
//        _msg.msg_namelen = sizeof(_address);
        _msg.msg_iov = &_msgiov;
        _msg.msg_iovlen = 1;

        return true;
    }

    bool Rtnetlink::request(uint32_t type)
    {
        struct nl_req_t
        {
            nlmsghdr hdr;
            rtgenmsg gen;
        };

        nl_req_t req;
        memset(&req, 0, sizeof(req));
        req.hdr.nlmsg_len   = NLMSG_LENGTH(sizeof(rtgenmsg));
        req.hdr.nlmsg_type  = type;
        req.hdr.nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
        req.hdr.nlmsg_seq   = 1;
        req.hdr.nlmsg_pid   = getpid();
        req.gen.rtgen_family= AF_PACKET;

        iovec iov;
        iov.iov_base    = &req;
        iov.iov_len     = req.hdr.nlmsg_len;

        sockaddr_nl address;
        memset(&address, 0, sizeof(address));
        address.nl_family = AF_NETLINK;

        msghdr msg;
        memset(&msg, 0, sizeof(msg));
        msg.msg_iov     = &iov;
        msg.msg_iovlen  = 1;
        msg.msg_name    = &address;
        msg.msg_namelen = sizeof(address);

        assert(_sock);
        if(0 > sendmsg(*_sock, &msg, 0))
        {
            LOGE("rtnetlink send: "<<strerror(errno));
            return false;
        }

        return true;
    }

    namespace
    {
        struct LinkAttrs
        {
            std::string     _name;
            uint32          _mtu{0};
            bool            _wireless{false};
        };

        void fetchRta(rtattr *rta, uint32 len, LinkAttrs &res)
        {
            while(RTA_OK(rta, len))
            {
                switch(rta->rta_type)
                {
                case IFLA_IFNAME:
                    res._name = (char *)RTA_DATA(rta);
                    break;
                case IFLA_MTU:
                    res._mtu = *(int *)RTA_DATA(rta);
                    break;
                case IFLA_WIRELESS:
                    res._wireless = true;
                    break;
                }

                rta = RTA_NEXT(rta, len);
            }
        }

        void fetchRta(rtattr *rta, uint32 len, net::ip4::LinkAddress &address, uint8_t prefixlen)
        {
            memset(address.address.octets.data(), 0, 4);
            memset(address.netmask.octets.data(), 0, 4);
            memset(address.broadcast.octets.data(), 0, 4);

            for(std::size_t i(0); i<prefixlen && i<32; i++)
            {
                address.netmask.octets[i/8] |= (1<<i%8);
            }

            while(RTA_OK(rta, len))
            {
                switch(rta->rta_type)
                {
                case IFA_LOCAL:
                    memcpy(address.address.octets.data(), RTA_DATA(rta), 4);
                    break;
                case IFA_BROADCAST:
                    memcpy(address.broadcast.octets.data(), RTA_DATA(rta), 4);
                    break;
                }

                rta = RTA_NEXT(rta, len);
            }
        }

        void fetchRta(rtattr *rta, uint32 len, net::ip6::LinkAddress &address, uint8_t prefixlen, uint8_t scope)
        {
            address.prefixLength = prefixlen;

            switch(scope)
            {
            case RT_SCOPE_UNIVERSE:
                address.scope = net::ip6::LinkAddress::Scope::global;
                break;
            case RT_SCOPE_SITE:
                address.scope = net::ip6::LinkAddress::Scope::link;
                break;
            case RT_SCOPE_LINK:
                address.scope = net::ip6::LinkAddress::Scope::link;
                break;
            case RT_SCOPE_HOST:
                address.scope = net::ip6::LinkAddress::Scope::host;
                break;
            case RT_SCOPE_NOWHERE:
            default:
                address.scope = net::ip6::LinkAddress::Scope::null;
                break;
            }

            memset(address.address.octets.data(), 0, 16);

            while(RTA_OK(rta, len))
            {
                switch(rta->rta_type)
                {
                case IFA_ADDRESS:
                    memcpy(address.address.octets.data(), RTA_DATA(rta), 16);
                    break;
                }

                rta = RTA_NEXT(rta, len);
            }
        }
    }

    bool Rtnetlink::read()
    {
        assert(_sock);
        if(0 == dci::async::acquireAny(_stop, _sock->readyEvent()))
        {
            return false;
        }

        auto sflags = _sock->seizeReadyState();
        if(dci::poller::Descriptor::rsf_error & sflags)
        {
            LOGE("rtnetlink poll: "<<_sock->error());
            return false;
        }

        if(!(dci::poller::Descriptor::rsf_read & sflags))
        {
            return true;
        }

        for(;;)
        {
            ssize_t readedSize = recvmsg(*_sock, &_msg, MSG_DONTWAIT);

            if(0 >= readedSize)
            {
                if(EINTR == errno || EAGAIN == errno)
                {
                    return true;
                }

                LOGE("rtnetlink recvmsg: "<<strerror(errno));
                return false;
            }

            for(
                nlmsghdr *h = (nlmsghdr *)_msgiov.iov_base;
                NLMSG_OK(h, readedSize);
                h = NLMSG_NEXT(h, readedSize))
            {
                switch (h->nlmsg_type)
                {
                case RTM_NEWLINK:
                    {
                        ifinfomsg *ifi = (ifinfomsg*) NLMSG_DATA(h);
                        Link *link = getLink(ifi->ifi_index);
                        bool isNew = !link;

                        LinkAttrs attrs;
                        fetchRta(IFLA_RTA(ifi), h->nlmsg_len, attrs);

                        if(!link)
                        {
                            if(!ifi->ifi_change && attrs._wireless)
                            {
                                //ignore wireless events for absent devices
                                continue;
                            }

                            link = addLink(ifi->ifi_index);
                        }

                        link->setName(attrs._name);
                        link->setMtu(attrs._mtu);

                        {
                            uint32 flags = 0;

                            if(ifi->ifi_flags & IFF_UP)         flags |= (uint32)net::Link::Flags::up;
                            if(ifi->ifi_flags & IFF_RUNNING)    flags |= (uint32)net::Link::Flags::running;
                            if(ifi->ifi_flags & IFF_BROADCAST)  flags |= (uint32)net::Link::Flags::broadcast;
                            if(ifi->ifi_flags & IFF_LOOPBACK)   flags |= (uint32)net::Link::Flags::loopback;
                            if(ifi->ifi_flags & IFF_POINTOPOINT)flags |= (uint32)net::Link::Flags::p2p;
                            if(ifi->ifi_flags & IFF_MULTICAST)  flags |= (uint32)net::Link::Flags::multicast;

                            link->setFlags(flags);
                        }

                        if(isNew)
                        {
                            host()->onLinkAdded(ifi->ifi_index, link);
                        }
                    }
                    break;

                case RTM_DELLINK:
                    {
                        ifinfomsg *ifi = (ifinfomsg*) NLMSG_DATA(h);
                        delLink(ifi->ifi_index);
                    }
                    break;

                case RTM_NEWADDR:
                case RTM_DELADDR:
                    {
                        ifaddrmsg *ifa = (ifaddrmsg *) NLMSG_DATA(h);
                        Link *link = getLink(ifa->ifa_index);
                        assert(link);

                        if(!link)
                        {
                            break;
                        }

                        switch(ifa->ifa_family)
                        {
                        case AF_INET:
                            {
                                net::ip4::LinkAddress address;
                                fetchRta(IFA_RTA(ifa), h->nlmsg_len, address, ifa->ifa_prefixlen);

                                if(RTM_NEWADDR == h->nlmsg_type)
                                {
                                    link->addIp4(address);
                                }
                                else
                                {
                                    link->delIp4(address);
                                }
                            }
                            break;
                        case AF_INET6:
                            {
                                net::ip6::LinkAddress address;
                                fetchRta(IFA_RTA(ifa), h->nlmsg_len, address, ifa->ifa_prefixlen, ifa->ifa_scope);

                                if(RTM_NEWADDR == h->nlmsg_type)
                                {
                                    link->addIp6(address);
                                }
                                else
                                {
                                    link->delIp6(address);
                                }
                            }
                        default:
                            break;
                        }
                    }
                    break;

                default:
                    break;
                }
            }
        }

        return true;
    }

    void Rtnetlink::shutdown()
    {
        _sock.reset();

        if(_msgiov.iov_base)
        {
            free(_msgiov.iov_base);
            _msgiov.iov_base = nullptr;
        }
    }

    void Rtnetlink::worker()
    {
        bool work = startup();

        if(work)
        {
            work &=
                request(RTM_GETLINK) &&
                read() &&
                request(RTM_GETADDR) &&
                read();
        }

        _initialized.set();

        if(work)
        {
            while(read());
        }

        shutdown();

        _stopped.set();
    }


}
