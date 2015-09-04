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
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< > ServiceHub::start(Channel &&arg_0)
    {
        assert(0);
        (void)arg_0;
        return Future< >(std::make_error_code(std::errc::address_family_not_supported));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< Channel> ServiceHub::stop()
    {
        assert(0);
        return Future< Channel>(std::make_error_code(std::errc::address_family_not_supported));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< ServiceHub::ServiceId> ServiceHub::inject(Interface &&arg_0)
    {
        assert(0);
        (void)arg_0;
        return Future< ServiceId>(std::make_error_code(std::errc::address_family_not_supported));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< Interface, ServiceHub::ServiceId> ServiceHub::invite(Iid &&arg_0)
    {
        assert(0);
        (void)arg_0;
        return Future< Interface, ServiceId>(std::make_error_code(std::errc::address_family_not_supported));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< Interface> ServiceHub::eject(ServiceId &&arg_0)
    {
        assert(0);
        (void)arg_0;
        return Future< Interface>(std::make_error_code(std::errc::address_family_not_supported));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void ServiceHubHandlerFactory::createService(void *outFuture)
    {
        auto *res = static_cast<dci::async::Future<std::error_code, ::streamer::ServiceHub> *>(outFuture);
        *res = ::streamer::ServiceHub(* new ServiceHub);
    }

}
