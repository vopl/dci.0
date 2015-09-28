#pragma once

#include <dci/site/serviceFactory.hpp>
#include "streamer.hpp"
#include "streamerHandlerSkel.hpp"

#include <experimental/optional>

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

        Future< > start(Channel &&arg_0);
        Future< Channel> stop();

        Future< ChannelHub::ChannelId> inject(ChannelOpposite &&arg_0);
        Future< ChannelOpposite> eject(ChannelHub::ChannelId &&arg_0);
    };

    struct ChannelHubHandlerFactory
        : ::dci::site::ServiceFactory
    {
        void createService(void *outFuture) override;
    };

}
