#pragma once

#include <dci/couple/runtime.hpp>
#include "streamer.hpp"

namespace impl
{
    using namespace dci::couple::runtime;
    using namespace streamer;

    template <class Derived>
    class ChannelHub
    {

    public:
        Future< streamer::ChannelHub::SubchannelId> injectSubchannel(Channel &&arg_0);
        Future< Channel> ejectSubchannel(streamer::ChannelHub::SubchannelId &&arg_0);
    };
}
