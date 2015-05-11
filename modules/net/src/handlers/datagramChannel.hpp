#pragma once

#include "scopeSelector.hpp"

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace net;

    template <class Address>
    struct DatagramChannelHandler
        : ScopeSelector<Address>::datagram::template ChannelHandlerSkel<DatagramChannelHandler<Address>>
    {
        Future< > bind(Address &&v)
        {
            assert(0);
        }

        Future< ::Data> receiveData()
        {
            assert(0);
        }

        Future< Data, Address> receive()
        {
            assert(0);
        }

        Future< > send(::Data &&d, Address &&a)
        {
            assert(0);
        }

        Future< > close()
        {
            assert(0);
        }

    };
}
