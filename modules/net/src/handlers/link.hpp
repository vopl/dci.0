#pragma once

#include "net.hpp"
#include "netHandlerSkel.hpp"

namespace impl
{
    class Link;
}

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace net;

    class Link
        : public generated::handlers::net::Link<Link>
    {
    public:
        Link(::impl::Link *impl);
        ~Link();

        void dropImpl();

        Future< uint32> id();
        Future< string> name();
        Future< uint32> flags();
        Future< uint32> mtu();
        Future< list< ip4::LinkAddress>> ip4();
        Future< list< ip6::LinkAddress>> ip6();

    private:
        ::impl::Link *_impl;
    };
}
