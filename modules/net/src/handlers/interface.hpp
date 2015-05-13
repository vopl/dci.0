#pragma once

#include "net.hpp"
#include "netHandlerSkel.hpp"

namespace impl
{
    class Interface;
}

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace net;

    struct Interface
        : hs::net::Interface<Interface>
    {
        Interface(::impl::Interface *impl);
        ~Interface();

        void dropImpl();

        Future< uint32> flags();
        Future< uint32> mtu();
        Future< string> name();
        Future< list< ip4::Net>> ip4Nets();
        Future< list< ip6::Net>> ip6Nets();

    private:
        ::impl::Interface *_impl;
    };
}
