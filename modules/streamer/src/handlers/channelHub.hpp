#pragma once

#include <dci/site/serviceFactory.hpp>
#include "streamer.hpp"
#include "streamerHandlerSkel.hpp"

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace streamer;

    class ChannelHub
        : public generated::handlers::streamer::ChannelHub<ChannelHub>
    {
    public:
        ChannelHub();
        ~ChannelHub();

        Future< > attachChannel(Channel &&arg_0);
        Future< Channel> detachChannel();

        Future< SubchannelId> injectSubchannel(Channel &&arg_0);
        Future< Channel> ejectSubchannel(SubchannelId &&arg_0);
    };

    struct ChannelHubHandlerFactory
        : ::dci::site::ServiceFactory
    {
        void createService(void *outFuture) override;
    };

}
