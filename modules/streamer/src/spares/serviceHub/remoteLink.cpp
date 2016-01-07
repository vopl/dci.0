#include "remoteLink.hpp"

namespace spares { namespace serviceHub
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    RemoteLink::RemoteLink()
    {

    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    RemoteLink::~RemoteLink()
    {

    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void RemoteLink::stop()
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future<streamer::ServiceHub::Remote> RemoteLink::eject()
    {
        assert(0);
        return Future<streamer::ServiceHub::Remote>(make_error_code(std::errc::address_not_available));
    }

}}
