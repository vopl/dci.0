#pragma once

#include "net.hpp"
#include "netHandlerSkel.hpp"

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace net;

    struct InterfaceHandler
        : hs::net::Interface<InterfaceHandler>
    {
        Future< uint32> flags();
        Future< uint32> mtu();
        Future< string> name();
        Future< list< ip4::Net>> ip4Nets();
        Future< list< ip6::Net>> ip6Nets();
    };
}
