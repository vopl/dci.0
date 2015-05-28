#pragma once

#include "scopeSelector.hpp"

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace net;

    template <class Address>
    struct DatagramChannel
        : ScopeSelector<Address>::datagram::template ChannelHandlerSkel<DatagramChannel<Address>>
    {
        Future< > bind(Address &&v)
        {
            (void)v;
            assert(0);
        }

        Future< Bytes> receiveData()
        {
            assert(0);
        }

        Future< Bytes, Address> receive()
        {
            assert(0);
        }

        Future< > send(Bytes &&d, Address &&a)
        {
            (void)d;
            (void)a;
            assert(0);
        }

        Future< > close()
        {
            assert(0);
        }

    };
}
