#include "hub.hpp"
#include <dci/couple/runtime.hpp>

namespace handlers
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Hub::Hub()
//        : _state(State::stop)
//        , _channelEndpoint(std::experimental::nullopt)
//        , _bo(ByteOrder::unknown)
//        , _version(0)
        : _channelEndpoint(std::experimental::nullopt)
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Hub::~Hub()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future<> Hub::start(Channel &&ch)
    {
        assert(0);
//        reset();
//        _state = State::negotiation;
//        _channelEndpoint = ChannelOpposite(ch);

//        //negotiation
//        /*
//         *
//         * версия протокола
//         * порядок байт
//         * порядок сторон
//         *
//         */

        return Future<>();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< > Hub::stop()
    {
        reset();
        return Future<>();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void_ Hub::put(Interface &&ch)
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< Interface> Hub::get(Iid &&iid)
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void_ Hub::join(Channel &&ch)
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Hub::reset()
    {
        assert(0);
//        if(State::stop != _state)
//        {
//            _state = State::stop;
//            if(_channelEndpoint)
//            {
//                _channelEndpoint = std::experimental::nullopt;
//            }

//            _bo = ByteOrder::unknown;
//            _version = 0;
//        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void HubHandlerFactory::createService(void *outFuture)
    {
        auto *res = static_cast<dci::async::Future<std::error_code, ::streamer::Hub> *>(outFuture);
        *res = ::streamer::Hub(* new Hub);
    }

}
