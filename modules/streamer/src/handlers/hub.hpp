#pragma once

#include <dci/site/serviceFactory.hpp>
#include "streamer.hpp"
#include "streamerHandlerSkel.hpp"

#include <experimental/optional>

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace streamer;

    class Hub
        : public hs::streamer::Hub<Hub>
    {
    public:
        Hub();
        ~Hub();

        Future< Hub::SubchannelId> attach(Channel &&unnamed_0);
        Future< > detach(Hub::SubchannelId &&unnamed_0);

        Future< > start(Channel &&unnamed_0);
        Future< > stop();

        void_ flow(Channel::Traffic &&unnamed_0);

    };

    struct HubHandlerFactory
        : ::dci::site::ServiceFactory
    {
        void createService(void *outFuture) override;
    };

}
