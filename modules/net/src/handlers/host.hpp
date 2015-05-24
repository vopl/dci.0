#pragma once

#include <dci/site/serviceFactory.hpp>

#include "net.hpp"
#include "netHandlerSkel.hpp"

namespace impl
{
    class Host;
}

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace net;

    struct Host
        : hs::net::Host<Host>
    {
        Host(::impl::Host *impl);
        ~Host();

        void dropImpl();

        Future<map<uint32, ::net::Link>> links();

        Future< ip4::stream::Server> ip4StreamServer();
        Future< ip4::stream::Client> ip4StreamClient();
        Future< ip4::datagram::Channel> ip4DatagramChannel();

        Future< ip6::stream::Server> ip6StreamServer();
        Future< ip6::stream::Client> ip6StreamClient();
        Future< ip6::datagram::Channel> ip6DatagramChannel();

        Future< local::stream::Server> localStreamServer();
        Future< local::stream::Client> localStreamClient();

    private:
        ::impl::Host *_impl;
    };

    struct HostHandlerFactory
        : dci::site::ServiceFactory
    {
        void createService(void *outFuture) override;
    };
}
