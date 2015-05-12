#pragma once

#include "scopeSelector.hpp"

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace net;

    template <class Address>
    struct StreamChannelHandler
        : ScopeSelector<Address>::stream::template ChannelHandlerSkel<StreamChannelHandler<Address>>
    {

        Future< Address> ownAddress()
        {
            assert(0);
        }

        Future< Address> peerAddress()
        {
            assert(0);
        }

        Future< ::Data> read()
        {
            assert(0);
        }

        Future< > write(::Data &&v)
        {
            (void)v;
            assert(0);
        }

        Future< > close()
        {
            assert(0);
        }

    };
}
