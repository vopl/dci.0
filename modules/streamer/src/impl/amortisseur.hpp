#pragma once

#include <dci/couple/runtime.hpp>
#include "streamer.hpp"

namespace impl
{
    using namespace dci::couple::runtime;
    using namespace streamer;

    template <class Derived>
    class Amortisseur
    {

    public:
        Future< > startAmortisseur(Channel &&arg_0);
        Future< > pauseAmortisseur();
        Future< > resumeAmortisseur();
        Future< Channel> migrateAmortisseur(Channel &&arg_0);
        Future< Channel> stopAmortisseur();
        Future< ::streamer::Amortisseur::AmortisseurState> amortisseurState();
    };
}
