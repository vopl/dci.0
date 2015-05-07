#pragma once

#include "scopeSelector.hpp"
#include "streamChannel.hpp"

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace net;

    template <class Address>
    struct StreamServerHandler
        : ScopeSelector<Address>::stream::template ServerHandlerSkel<StreamServerHandler<Address>>
    {
        using Channel = typename ScopeSelector<Address>::stream::Channel;

        Future< > bind(Address &&v)
        {
            assert(0);
        }

        Future< > listen()
        {
            assert(0);
        }

        Future< Channel> accept()
        {
            assert(0);

            auto *v = new StreamChannelHandler<Address>;
            return Channel(*v);
        }

        Future< > close()
        {
            assert(0);
        }

    };
}
