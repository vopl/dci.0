#include "channelHub.hpp"
#include <dci/couple/runtime.hpp>

namespace handlers
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    ChannelHub::ChannelHub()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    ChannelHub::~ChannelHub()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< > ChannelHub::start(Channel &&arg_0)
    {
        assert(0);
        (void)arg_0;
        return Future< >(std::make_error_code(std::errc::address_family_not_supported));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< Channel> ChannelHub::stop()
    {
        assert(0);
        return Future< Channel>(std::make_error_code(std::errc::address_family_not_supported));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< ChannelHub::ChannelId> ChannelHub::inject(ChannelOpposite &&arg_0)
    {
        assert(0);
        (void)arg_0;
        return Future< ChannelHub::ChannelId>(std::make_error_code(std::errc::address_family_not_supported));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< ChannelOpposite> ChannelHub::eject(ChannelHub::ChannelId &&arg_0)
    {
        assert(0);
        (void)arg_0;
        return Future< ChannelOpposite>(std::make_error_code(std::errc::address_family_not_supported));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void ChannelHubHandlerFactory::createService(void *outFuture)
    {
        auto *res = static_cast<dci::async::Future<std::error_code, ::streamer::ChannelHub> *>(outFuture);
        *res = ::streamer::ChannelHub(* new ChannelHub);
    }

}
