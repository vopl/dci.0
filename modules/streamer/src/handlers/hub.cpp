#include "hub.hpp"
#include <dci/couple/runtime.hpp>

namespace handlers
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Hub::Hub()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Hub::~Hub()
    {
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void HubHandlerFactory::createService(void *outFuture)
    {
        auto *res = static_cast<dci::async::Future<std::error_code, ::streamer::Hub> *>(outFuture);
        *res = ::streamer::Hub(* new Hub);
    }

}
