#include "interface.hpp"
#include "../handlers/interface.hpp"

namespace impl
{
    Interface::Interface()
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


}
