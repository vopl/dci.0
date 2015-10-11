#include "channelHub.hpp"

namespace impl
{
    template <class Derived>
    Future< streamer::ChannelHub::SubchannelId> ChannelHub<Derived>::injectSubchannel(Channel &&arg_0)
    {
        (void)arg_0;
        assert(0);
        return Future< streamer::ChannelHub::SubchannelId>(make_error_code(std::errc::address_not_available));
    }

    template <class Derived>
    Future< Channel> ChannelHub<Derived>::ejectSubchannel(streamer::ChannelHub::SubchannelId &&arg_0)
    {
        (void)arg_0;
        assert(0);
        return Future< Channel>(make_error_code(std::errc::address_not_available));
    }
}

#include "../handlers/channelHub.hpp"
template class impl::ChannelHub<handlers::ChannelHub>;
