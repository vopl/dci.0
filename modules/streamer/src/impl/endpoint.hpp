#pragma once

#include <dci/couple/runtime.hpp>
#include <dci/async.hpp>
#include "streamer.hpp"
#include "endpoint/message.hpp"
#include "endpoint/inputFlowParser.hpp"

namespace impl
{
    using namespace dci::couple::runtime;
    using namespace streamer;

    //////////////////////////////////////////////////////////////////
    //events

    //general
    struct attach {};
    struct detach {};

    struct null {};
    struct unknown {};

    //channel
    struct hello {};
    struct bye {};

    //links
//    struct links_create {};
//    struct links_destroy {};
//    struct links_generation {};
//    struct links_commitGeneration {};


    //link
    struct link_request {};
    struct link_response {};
    struct link_message {};

    //serviceHub
    struct link_inject {};
    struct link_invite {};
    struct link_eject {};

    //amortisseur
    struct recovery {};



    template <class Derived>
    class Endpoint
    {
    public:
        Endpoint();
        ~Endpoint();

    public:
        Future< > attachChannel(Channel &&arg_0);
        Future< Channel> detachChannel();

    public:
        Future<> write(endpoint::Message &&msg);
        Future<endpoint::Message> read();

    private:
        void requestInputFlowIfNeed();
        void requestInputRemit();
        void onInputFlowed(Future<Bytes> &f);
        void onInputRemitted(Future<> &f);

        Future<Bytes> onOutputFlow();
        Future<> onOutputRemit();

        void resolveDetach();

    private:
        struct Deleter
        {
            template <class T>
            void operator()(T *v)
            {
                delete v;
            }
        };

        struct InputMessage
            : public dci::mm::IntrusiveDlistElement<InputMessage>
            , public dci::mm::NewDelete<InputMessage>
        {
            endpoint::Message _message;
        };

        struct InputReadRequest
            : public dci::mm::IntrusiveDlistElement<InputReadRequest>
            , public dci::mm::NewDelete<InputReadRequest>
        {
            Promise<endpoint::Message>    _promise;
        };

        struct OutputWriteRequest
            : public dci::mm::IntrusiveDlistElement<OutputWriteRequest>
            , public dci::mm::NewDelete<OutputWriteRequest>
        {
            Bytes       _data;
            Promise<>   _promise;
        };

        struct OutputFlowRequest
            : public dci::mm::IntrusiveDlistElement<OutputFlowRequest>
            , public dci::mm::NewDelete<OutputFlowRequest>
        {
            Promise<Bytes>  _promise;
        };


    private:
        Channel             _channel;
        Promise<Channel>    _detachPromise{dci::async::PromiseNullInitializer()};

        //input waiters
        std::size_t         _inputFlowRequested{0};
        std::size_t         _inputRemitRequested{0};

        //input
        endpoint::InputFlowParser                               _inputFlowParser;
        dci::mm::IntrusiveDlist<InputMessage, Deleter>          _inputMessagesAccumuler;
        dci::mm::IntrusiveDlist<InputReadRequest, Deleter>      _inputReadRequests;

        //output
        dci::mm::IntrusiveDlist<OutputWriteRequest, Deleter>    _outputWriteRequests;
        dci::mm::IntrusiveDlist<OutputFlowRequest, Deleter>     _outputFlowRequests;
    };
}
