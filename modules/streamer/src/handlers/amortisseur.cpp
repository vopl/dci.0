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
    void AmortisseurHandlerFactory::createService(void *outFuture)
    {
        auto *res = static_cast<dci::async::Future<std::error_code, ::streamer::Amortisseur> *>(outFuture);
        *res = ::streamer::Amortisseur(* new Amortisseur);
    }
}
