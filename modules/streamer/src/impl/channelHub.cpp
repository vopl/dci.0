#include "channelHub.hpp"

namespace impl
{
    template <class Derived>
    Future< streamer::ChannelHub::SubchannelId> ChannelHub<Derived>::injectSubchannel(Channel &&arg_0)
    {
        assert(0);
    }

    template <class Derived>
    Future< Channel> ChannelHub<Derived>::ejectSubchannel(streamer::ChannelHub::SubchannelId &&arg_0)
    {
        assert(0);
    }
}

#include "../handlers/channelHub.hpp"
template class impl::ChannelHub<handlers::ChannelHub>;
