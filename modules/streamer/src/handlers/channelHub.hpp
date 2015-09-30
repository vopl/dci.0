#pragma once

#include <dci/site/serviceFactory.hpp>
#include "streamer.hpp"
#include "streamerHandlerSkel.hpp"

#include "../impl/endpoint.hpp"
#include "../impl/channelHub.hpp"

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace streamer;

    class ChannelHub
        : public generated::handlers::streamer::ChannelHub<ChannelHub>
        , public ::impl::Endpoint<ChannelHub>
        , public ::impl::ChannelHub<ChannelHub>
    {
    public:
        ChannelHub();
        ~ChannelHub();
    };

    struct ChannelHubHandlerFactory
        : ::dci::site::ServiceFactory
    {
        void createService(void *outFuture) override;
    };

}
