#pragma once

#include "scopeSelector.hpp"

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace net;

    template <class Address>
    struct StreamChannel
        : ScopeSelector<Address>::stream::template ChannelHandlerSkel<StreamChannel<Address>>
    {

        Future< Address> self()
        {
            assert(0);
        }

        Future< Address> peer()
        {
            assert(0);
        }

        Future< Bytes> read()
        {
            assert(0);
        }

        Future< > write(Bytes &&v)
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
