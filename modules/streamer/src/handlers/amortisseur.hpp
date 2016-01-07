#pragma once

#include <dci/site/serviceFactory.hpp>
#include "streamer.hpp"
#include "streamerHandlerSkel.hpp"

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace streamer;

    class Amortisseur
        : public generated::handlers::streamer::Amortisseur<Amortisseur>
    {
    public:
        Amortisseur();
        ~Amortisseur();

        Future< > attachChannel(Channel &&arg_0);
        Future< Channel> detachChannel();

        Future< > startAmortisseur(Channel &&arg_0);
        Future< > pauseAmortisseur();
        Future< > resumeAmortisseur();
        Future< Channel> migrateAmortisseur(Channel &&arg_0);
        Future< Channel> stopAmortisseur();
        Future< AmortisseurState> amortisseurState();
    };

    struct AmortisseurHandlerFactory
        : ::dci::site::ServiceFactory
    {
        void createService(void *outFuture) override;
    };

}
