#pragma once

#include <dci/site/serviceFactory.hpp>
#include "streamer.hpp"
#include "streamerHandlerSkel.hpp"

#include "../impl/fsm/endpoint.hpp"
#include <boost/msm/back/state_machine.hpp>

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

        //endpoint
        Future< > attachChannel(Channel &&arg_0);
        Future< Channel> detachChannel();

        //serviceHub
        Future< streamer::ServiceHub::ServiceId> injectService(Interface &&arg_0);
        Future< Interface, streamer::ServiceHub::ServiceId> inviteService(Iid &&arg_0);
        Future< Interface> ejectService(streamer::ServiceHub::ServiceId &&arg_0);

    private:

        using FSM = ::impl::fsm::Endpoint;
        FSM _fsm;
    };

    struct ServiceHubHandlerFactory
        : ::dci::site::ServiceFactory
    {
        void createService(void *outFuture) override;
    };

}
