#pragma once

#include "net.hpp"
#include <set>

namespace handlers
{
    class Link;
}

namespace impl
{
    using namespace dci::couple::runtime;
    using namespace net;

    class Link
    {
    public:
        Link(uint32 id);
        ~Link();

        void registerHandler(handlers::Link *handler);
        void unregisterHandler(handlers::Link *handler);

        uint32 id() const;
        const string &name() const;
        uint32 flags() const;
        uint32 mtu() const;
        const list< ip4::LinkAddress> &ip4() const;
        const list< ip6::LinkAddress> &ip6() const;

        void setName(string v);
        void setFlags(uint32 v);
        void setMtu(uint32 v);

        void setIp4(const list< ip4::LinkAddress> &v);
        void setIp6(const list< ip6::LinkAddress> &v);

        void addIp4(const ip4::LinkAddress &v);
        void delIp4(const ip4::LinkAddress &v);

        void addIp6(const ip6::LinkAddress &v);
        void delIp6(const ip6::LinkAddress &v);

    private:
        template <class M>
        void flushChanges(M m);

    private:
        using Handlers = std::set<handlers::Link *>;
        Handlers _handlers;

        uint32          _id;
        string          _name;
        uint32          _flags;
        uint32          _mtu;
        list< ip4::LinkAddress> _ip4;
        list< ip6::LinkAddress> _ip6;
    };
}
