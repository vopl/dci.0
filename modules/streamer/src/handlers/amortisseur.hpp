#pragma once

#include <dci/site/serviceFactory.hpp>
#include "streamer.hpp"
#include "streamerHandlerSkel.hpp"

#include "../impl/endpoint.hpp"
#include "../impl/amortisseur.hpp"

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace streamer;

    class Amortisseur
        : public generated::handlers::streamer::Amortisseur<Amortisseur>
        , public ::impl::Endpoint<Amortisseur>
        , public ::impl::Amortisseur<Amortisseur>
    {
    public:
        Amortisseur();
        ~Amortisseur();

        using ::impl::Amortisseur<Amortisseur>::amortisseurState;
    };

    struct AmortisseurHandlerFactory
        : ::dci::site::ServiceFactory
    {
        void createService(void *outFuture) override;
    };

}
