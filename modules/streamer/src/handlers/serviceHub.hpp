#pragma once

#include <dci/site/serviceFactory.hpp>
#include "streamer.hpp"
#include "streamerHandlerSkel.hpp"

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace streamer;

    class ServiceHub
        : public generated::handlers::streamer::ServiceHub<ServiceHub>
    {
    public:
        ServiceHub();
        ~ServiceHub();

        Future< > start(Channel &&arg_0);
        Future< Channel> stop();

        Future< ServiceId> inject(Interface &&arg_0);
        Future< Interface, ServiceId> invite(Iid &&arg_0);
        Future< Interface> eject(ServiceId &&arg_0);
    };

    struct ServiceHubHandlerFactory
        : ::dci::site::ServiceFactory
    {
        void createService(void *outFuture) override;
    };

}
