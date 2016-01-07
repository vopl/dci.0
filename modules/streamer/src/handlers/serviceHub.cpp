#include "serviceHub.hpp"
#include <dci/couple/runtime.hpp>

namespace handlers
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    ServiceHub::ServiceHub()
        : _stack()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    ServiceHub::~ServiceHub()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< > ServiceHub::attachChannel(Channel &&arg_0)
    {
        return _stack.attachChannel(std::forward<Channel>(arg_0));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< Channel> ServiceHub::detachChannel()
    {
        return _stack.detachChannel();
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< ServiceHub::ServiceId> ServiceHub::injectService(Remote &&arg_0)
    {
        return _stack.injectService(std::forward<Remote>(arg_0));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< ServiceHub::Local, ServiceHub::ServiceId> ServiceHub::inviteService(Iid &&arg_0)
    {
        return _stack.inviteService(std::forward<Iid>(arg_0));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< ServiceHub::Remote> ServiceHub::ejectService(ServiceId &&arg_0)
    {
        return _stack.ejectService(std::forward<ServiceId>(arg_0));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void ServiceHubHandlerFactory::createService(void *outFuture)
    {
        auto *res = static_cast<dci::async::Future<std::error_code, ::streamer::ServiceHub> *>(outFuture);
        *res = ::streamer::ServiceHub(* new ServiceHub);
    }

}
