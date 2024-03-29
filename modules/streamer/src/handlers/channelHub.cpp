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
    Future< > ChannelHub::attachChannel(Channel &&arg_0)
    {
        (void)arg_0;
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< Channel> ChannelHub::detachChannel()
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< ChannelHub::SubchannelId> ChannelHub::injectSubchannel(Channel &&arg_0)
    {
        (void)arg_0;
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< Channel> ChannelHub::ejectSubchannel(SubchannelId &&arg_0)
    {
        (void)arg_0;
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void ChannelHubHandlerFactory::createService(void *outFuture)
    {
        auto *res = static_cast<dci::async::Future<std::error_code, ::streamer::ChannelHub> *>(outFuture);
        *res = ::streamer::ChannelHub(* new ChannelHub);
    }
}
