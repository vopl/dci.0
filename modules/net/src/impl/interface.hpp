#pragma once

#include "net.hpp"
#include <set>

namespace handlers
{
    class Interface;
}

namespace impl
{
    using namespace dci::couple::runtime;
    using namespace net;

    class Interface
    {
    public:
        Interface();
        ~Interface();

        void registerHandler(handlers::Interface *handler);
        void unregisterHandler(handlers::Interface *handler);

        uint32 flags() const;
        uint32 mtu() const;
        const string &name() const;
        const list< ip4::Net> &ip4Nets() const;
        const list< ip6::Net> &ip6Nets() const;

    private:
        std::set<handlers::Interface *> _handlers;

        uint32          _flags;
        uint32          _mtu;
        string          _name;
        list< ip4::Net> _ip4Nets;
        list< ip6::Net> _ip6Nets;

    };
}
