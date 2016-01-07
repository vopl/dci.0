#include "localLink.hpp"

namespace spares { namespace serviceHub
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    LocalLink::LocalLink()
    {

    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    LocalLink::~LocalLink()
    {

    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void LocalLink::stop()
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future<ServiceHub::ServiceId> LocalLink::inject(streamer::ServiceHub::Remote &&arg_0)
    {
        (void)arg_0;
        assert(0);

        return Future<ServiceHub::ServiceId>(make_error_code(std::errc::address_not_available));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future<streamer::ServiceHub::Local, ServiceHub::ServiceId> LocalLink::invite(Iid &&arg_0)
    {
        (void)arg_0;
        assert(0);
        return Future<streamer::ServiceHub::Local, ServiceHub::ServiceId>(make_error_code(std::errc::address_not_available));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future<streamer::ServiceHub::Remote> LocalLink::eject()
    {
        assert(0);
        return Future<streamer::ServiceHub::Remote>(make_error_code(std::errc::address_not_available));
    }
}}
