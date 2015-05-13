#include "interface.hpp"
#include "../handlers/interface.hpp"

#include <dci/async.hpp>

namespace impl
{
    Interface::Interface(const string &name)
        : _flags()
        , _mtu()
        , _name(name)
    {
    }

    Interface::~Interface()
    {
        for(auto h : _handlers)
        {
            h->dropImpl();
        }
        _handlers.clear();
    }

    void Interface::registerHandler(handlers::Interface *handler)
    {
        _handlers.insert(handler);
    }

    void Interface::unregisterHandler(handlers::Interface *handler)
    {
        _handlers.erase(handler);
    }

    uint32 Interface::flags() const
    {
        return _flags;
    }

    uint32 Interface::mtu() const
    {
        return _mtu;
    }

    const string &Interface::name() const
    {
        return _name;
    }

    const list< ip4::Net> &Interface::ip4Nets() const
    {
        return _ip4Nets;
    }

    const list< ip6::Net> &Interface::ip6Nets() const
    {
        return _ip6Nets;
    }

    void Interface::setFlags(uint32 v)
    {
        if(_flags != v)
        {
            _flags = v;

            for(handlers::Interface *h : _handlers)
            {
                dci::async::spawn([&]()
                {
                    h->flagsChanged();
                });
            }
        }
    }

    void Interface::setMtu(uint32 v)
    {
        if(_mtu != v)
        {
            _mtu = v;

            for(handlers::Interface *h : _handlers)
            {
                dci::async::spawn([&]()
                {
                    h->mtuChanged();
                });
            }
        }
    }


    void Interface::setIp4Nets(const list< ip4::Net> &v)
    {
        if(_ip4Nets != v)
        {
            _ip4Nets = v;

            for(handlers::Interface *h : _handlers)
            {
                dci::async::spawn([&]()
                {
                    h->ip4NetsChanged();
                });
            }
        }
    }

    void Interface::setIp6Nets(const list< ip6::Net> &v)
    {
        if(_ip6Nets != v)
        {
            _ip6Nets = v;

            for(handlers::Interface *h : _handlers)
            {
                dci::async::spawn([&]()
                {
                    h->ip6NetsChanged();
                });
            }
        }
    }

}

namespace std
{
    inline bool operator==(const net::ip4::Net &a, const net::ip4::Net &b)
    {
        return
                a.address.octets == b.address.octets &&
                a.netmask.octets == b.netmask.octets &&
                a.broadcast.octets == b.broadcast.octets;
    }

    inline bool operator==(const net::ip6::Net &a, const net::ip6::Net &b)
    {
        return
                a.address.octets == b.address.octets &&
                a.prefixLength == b.prefixLength&&
                a.scopeId == b.scopeId;
    }
}
