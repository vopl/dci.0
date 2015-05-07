#pragma once

#include "scopeSelector.hpp"
#include "streamChannel.hpp"

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace net;

    template <class Address>
    struct StreamClientHandler
        : ScopeSelector<Address>::stream::template ClientHandlerSkel<StreamClientHandler<Address>>
    {
        using Channel = typename ScopeSelector<Address>::stream::Channel;

        Future< > bind(Address &&v)
        {
            assert(0);
        }

        Future< Channel> connect(Address &&address)
        {
            assert(0);

            auto *v = new StreamChannelHandler<Address>;
            return Channel(*v);
        }

        Future< > abort()
        {
            assert(0);
        }

    };
}
