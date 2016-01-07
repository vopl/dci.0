#pragma once

#include <dci/site/serviceFactory.hpp>
#include "streamer.hpp"
#include "streamerHandlerSkel.hpp"

#include "../spares/hub.hpp"
#include "../spares/serviceHub/localLink.hpp"
#include "../spares/serviceHub/remoteLink.hpp"

#include "../spares/accord.hpp"
#include "../spares/brim.hpp"

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace streamer;
    using namespace ::spares;
//    using namespace ::spares::hub;
    using namespace ::spares::serviceHub;

    class ServiceHub
        : public generated::handlers::streamer::ServiceHub<ServiceHub>
    {
    public:
        ServiceHub();
        ~ServiceHub();

        Future< > attachChannel(Channel &&arg_0);
        Future< Channel> detachChannel();
        // [out] ::dci::couple::runtime::Future< > channelFail(::streamer::error &&arg_0);

        Future< ServiceId> injectService(Remote &&arg_0);
        Future< Local, ServiceId> inviteService(Iid &&arg_0);
        Future< Remote> ejectService(ServiceId &&arg_0);
        // [out] ::dci::couple::runtime::void_ subchannelFail(::streamer::ChannelHub::SubchannelId &&arg_0, ::streamer::error &&arg_1);

    private:
        struct Stack
            : public Hub<Stack, serviceHub::LocalLink, serviceHub::RemoteLink>
            , public Accord<Stack>
            , public Brim<Stack>
        {
            using Hub<Stack, serviceHub::LocalLink, serviceHub::RemoteLink>::input;
            using Accord<Stack>::input;

        } _stack;
    };

    struct ServiceHubHandlerFactory
        : ::dci::site::ServiceFactory
    {
        void createService(void *outFuture) override;
    };
}
