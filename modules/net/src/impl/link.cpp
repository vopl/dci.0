#include "link.hpp"
#include "../handlers/link.hpp"

#include <dci/async.hpp>
#include <algorithm>

namespace
{
    inline bool address4Equal(const net::ip4::LinkAddress &a, const net::ip4::LinkAddress &b)
    {
        return
                a.address.octets == b.address.octets &&
                a.netmask.octets == b.netmask.octets &&
                a.broadcast.octets == b.broadcast.octets;
    }
    inline bool address6Equal(const net::ip6::LinkAddress &a, const net::ip6::LinkAddress &b)
    {
        return
                a.address.octets == b.address.octets &&
                a.prefixLength == b.prefixLength&&
                a.scope == b.scope;
    }

    struct AddressEqualPred
    {
        AddressEqualPred(const net::ip4::LinkAddress &a)
            : a4(a)
        {
        }

        AddressEqualPred(const net::ip6::LinkAddress &a)
            : a6(a)
        {
        }

        net::ip4::LinkAddress a4;
        net::ip6::LinkAddress a6;

        bool operator()(const net::ip4::LinkAddress &b) const
        {
            return address4Equal(a4, b);
        }

        bool operator()(const net::ip6::LinkAddress &b) const
        {
            return address6Equal(a6, b);
        }
    };
}

namespace impl
{
    Link::Link(uint32 id)
        : _id(id)
        , _name()
        , _flags()
        , _mtu()
    {
    }

    Link::~Link()
    {
        while(!_handlers.empty())
        {
            Handlers hs;
            hs.swap(_handlers);
            for(handlers::Link *h : hs)
            {
                h->dropImpl();
            }
        }
    }

    void Link::registerHandler(handlers::Link *handler)
    {
        _handlers.insert(handler);
    }

    void Link::unregisterHandler(handlers::Link *handler)
    {
        _handlers.erase(handler);
    }

    uint32 Link::id() const
    {
        return _id;
    }

    const string &Link::name() const
    {
        return _name;
    }

    uint32 Link::flags() const
    {
        return _flags;
    }

    uint32 Link::mtu() const
    {
        return _mtu;
    }

    const list< ip4::LinkAddress> &Link::ip4() const
    {
        return _ip4;
    }

    const list< ip6::LinkAddress> &Link::ip6() const
    {
        return _ip6;
    }

    void Link::setName(string v)
    {
        if(_name != v)
        {
            _name = v;
            flushChanges(&handlers::Link::nameChanged);
        }
    }

    void Link::setFlags(uint32 v)
    {
        if(_flags != v)
        {
            _flags = v;
            flushChanges(&handlers::Link::flagsChanged);
        }
    }

    void Link::setMtu(uint32 v)
    {
        if(_mtu != v)
        {
            _mtu = v;
            flushChanges(&handlers::Link::mtuChanged);

        }
    }

    void Link::setIp4(const list< ip4::LinkAddress> &v)
    {
        if(!std::equal(_ip4.begin(), _ip4.end(), v.begin(), v.end(), &address4Equal))
        {
            _ip4 = v;
            flushChanges(&handlers::Link::ip4Changed);
        }
    }

    void Link::setIp6(const list< ip6::LinkAddress> &v)
    {
        if(!std::equal(_ip6.begin(), _ip6.end(), v.begin(), v.end(), &address6Equal))
        {
            _ip6 = v;
            flushChanges(&handlers::Link::ip6Changed);
        }
    }

    void Link::addIp4(const ip4::LinkAddress &v)
    {
        auto iter = std::find_if(_ip4.begin(), _ip4.end(), AddressEqualPred(v));
        //assert(_ip4.end() == iter);

        if(_ip4.end() == iter)
        {
            _ip4.emplace_back(std::move(v));
            flushChanges(&handlers::Link::ip4Changed);
        }
    }

    void Link::delIp4(const ip4::LinkAddress &v)
    {
        auto iter = std::find_if(_ip4.begin(), _ip4.end(), AddressEqualPred(v));
        //assert(_ip4.end() != iter);

        if(_ip4.end() != iter)
        {
            _ip4.erase(iter);
            flushChanges(&handlers::Link::ip4Changed);
        }
    }

    void Link::addIp6(const ip6::LinkAddress &v)
    {
        auto iter = std::find_if(_ip6.begin(), _ip6.end(), AddressEqualPred(v));
        //assert(_ip6.end() == iter);

        if(_ip6.end() == iter)
        {
            _ip6.emplace_back(std::move(v));
            flushChanges(&handlers::Link::ip6Changed);
        }
    }

    void Link::delIp6(const ip6::LinkAddress &v)
    {
        auto iter = std::find_if(_ip6.begin(), _ip6.end(), AddressEqualPred(v));
        //assert(_ip6.end() != iter);

        if(_ip6.end() != iter)
        {
            _ip6.erase(iter);
            flushChanges(&handlers::Link::ip6Changed);
        }
    }

    template <class M>
    void Link::flushChanges(M m)
    {
        Handlers hs(_handlers);
        for(handlers::Link *h : hs)
        {
            (h->*m)();
        }
    }

}
