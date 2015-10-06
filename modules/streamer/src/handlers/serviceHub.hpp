#pragma once

#include <dci/site/serviceFactory.hpp>
#include "streamer.hpp"
#include "streamerHandlerSkel.hpp"

#include "../impl/endpoint.hpp"
#include "../impl/serviceHub.hpp"

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace streamer;

    class ServiceHub
        : public generated::handlers::streamer::ServiceHub<ServiceHub>
        , public ::impl::Endpoint<ServiceHub>
        , public ::impl::ServiceHub<ServiceHub>
    {
    public:
        ServiceHub();
        ~ServiceHub();

    private:
    };

    struct ServiceHubHandlerFactory
        : ::dci::site::ServiceFactory
    {
        void createService(void *outFuture) override;
    };

}
