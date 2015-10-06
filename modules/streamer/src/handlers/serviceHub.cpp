#include "serviceHub.hpp"
#include <dci/couple/runtime.hpp>

namespace handlers
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    ServiceHub::ServiceHub()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    ServiceHub::~ServiceHub()
    {
        detachChannel().wait();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void ServiceHubHandlerFactory::createService(void *outFuture)
    {
        auto *res = static_cast<dci::async::Future<std::error_code, ::streamer::ServiceHub> *>(outFuture);
        *res = ::streamer::ServiceHub(* new ServiceHub);
    }

}
