#pragma once

#include <dci/site/serviceFactory.hpp>
#include "streamer.hpp"
#include "streamerHandlerSkel.hpp"

#include <experimental/optional>

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace streamer;

    class Hub
        : public hs::streamer::Hub<Hub>
    {
    public:
        Hub();
        ~Hub();

        Future< >           start   (Channel &&ch);
        Future< >           stop    ();
        void_               put     (Interface &&ch);
        Future< Interface>  get     (Iid &&iid);
        void_               join    (Channel &&ch);

    private:
        void reset();

//    private:
//        enum class ByteOrder
//        {
//            unknown,
//            be,
//            le,
//        };

//        enum class State
//        {
//            stop,
//            negotiation,
//            work,
//            error,
//        };

        using ChannelEndpoint = std::experimental::optional<ChannelOpposite>;
//        using Protocol = std::experimental::optional<BinaryProtocol>;

    private:
//        State               _state;

        ChannelEndpoint     _channelEndpoint;

//        ByteOrder           _bo;
//        std::uint_fast32_t  _version;
    };

    struct HubHandlerFactory
        : ::dci::site::ServiceFactory
    {
        void createService(void *outFuture) override;
    };

}
