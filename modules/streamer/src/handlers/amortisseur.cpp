#include "amortisseur.hpp"
#include <dci/couple/runtime.hpp>

namespace handlers
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Amortisseur::Amortisseur()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Amortisseur::~Amortisseur()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< > Amortisseur::attachChannel(Channel &&arg_0)
    {
        (void)arg_0;
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< Channel> Amortisseur::detachChannel()
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< > Amortisseur::startAmortisseur(Channel &&arg_0)
    {
        (void)arg_0;
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< > Amortisseur::pauseAmortisseur()
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< > Amortisseur::resumeAmortisseur()
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< Channel> Amortisseur::migrateAmortisseur(Channel &&arg_0)
    {
        (void)arg_0;
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< Channel> Amortisseur::stopAmortisseur()
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< Amortisseur::AmortisseurState> Amortisseur::amortisseurState()
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void AmortisseurHandlerFactory::createService(void *outFuture)
    {
        auto *res = static_cast<dci::async::Future<std::error_code, ::streamer::Amortisseur> *>(outFuture);
        *res = ::streamer::Amortisseur(* new Amortisseur);
    }
}
