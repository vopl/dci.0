#pragma once

#include <dci/site/serviceFactory.hpp>

#include "net.hpp"
#include "netHandlerSkel.hpp"

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace net;

    struct HostHandler
        : hs::net::Host<HostHandler>
    {

        Future<list<Interface>> interfaces();
        Future< ip4::stream::Host> ip4StreamHost();
        Future< ip4::datagram::Host> ip4DatagramHost();
        Future< ip6::stream::Host> ip6StreamHost();
        Future< ip6::datagram::Host> ip6DatagramHost();
        Future< local::stream::Host> localStreamHost();
    };

    struct HostHandlerFactory
        : dci::site::ServiceFactory
    {
        void createService(void *outFuture) override;
    };
}
