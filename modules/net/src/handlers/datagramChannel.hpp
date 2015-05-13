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
