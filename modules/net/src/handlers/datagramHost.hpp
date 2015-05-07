#pragma once

#include "scopeSelector.hpp"
#include "datagramChannel.hpp"

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace net;

    template <class Address>
    struct DatagramHostHandler
        : ScopeSelector<Address>::datagram::template HostHandlerSkel<DatagramHostHandler<Address>>
    {
        using Channel = typename ScopeSelector<Address>::datagram::Channel;

        Future< Channel> mkChannel()
        {
            auto *v = new DatagramChannelHandler<Address>;
            return Channel(*v);
        }
    };

}
