#pragma once

#include "scopeSelector.hpp"
#include "datagramChannel.hpp"

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace net;

    template <class Address>
    struct DatagramHost
        : ScopeSelector<Address>::datagram::template HostHandlerSkel<DatagramHost<Address>>
    {
        using Channel = typename ScopeSelector<Address>::datagram::Channel;

        Future< Channel> mkChannel()
        {
            auto *v = new DatagramChannel<Address>;
            return Channel(*v);
        }
    };

}
