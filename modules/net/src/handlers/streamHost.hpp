#pragma once

#include "scopeSelector.hpp"
#include "streamServer.hpp"
#include "streamClient.hpp"

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace net;

    template <class Address>
    struct StreamHostHandler
        : ScopeSelector<Address>::stream::template HostHandlerSkel<StreamHostHandler<Address>>
    {
        using Server = typename ScopeSelector<Address>::stream::Server;
        using Client = typename ScopeSelector<Address>::stream::Client;

        Future< Server> mkServer()
        {
            auto *v = new StreamServerHandler<Address>;
            return Server(*v);
        }

        Future< Client> mkClient()
        {
            auto *v = new StreamClientHandler<Address>;
            return Client(*v);
        }
    };

}
