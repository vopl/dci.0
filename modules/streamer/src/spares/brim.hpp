#pragma once

#include <dci/couple/runtime.hpp>
#include <dci/async.hpp>
#include "streamer.hpp"
#include "protocol/parser.hpp"
#include "protocol/message.hpp"
#include "serialize.hpp"

#include <cstring>

namespace spares
{
    using namespace dci::couple::runtime;
    using namespace streamer;

    template <class Derived>
    class Brim
    {
    public:
        Brim();
        ~Brim();

        Future< > attachChannel(Channel &&arg_0);
        Future< Channel> detachChannel();

        template <class Message>
        Future< > output(Message &&message);

    private:
        void attachChannelComplete(std::error_code &&ec);
        void detachChannelComplete(std::error_code &&ec);
        Future<> onInput(Bytes &&data);

    private:
        Channel             _channel;
        Promise<>           _attachPromise{dci::async::PromiseNullInitializer()};
        Promise<Channel>    _detachPromise{dci::async::PromiseNullInitializer()};

        using Parser = protocol::Parser<
            protocol::Hello,
            protocol::Bye>;
        Parser    _parser;
    };











    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Derived>
    Brim<Derived>::Brim()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Derived>
    Brim<Derived>::~Brim()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Derived>
    Future< > Brim<Derived>::attachChannel(Channel &&arg_0)
    {
        if(!arg_0._input || !arg_0._output)
        {
            return make_error_code(streamer::error::badChannelValue);
        }

        if(_channel._input)
        {
            assert(_channel._output);
            return make_error_code(streamer::error::alreadyAttached);
        }

        if(_attachPromise)
        {
            return make_error_code(streamer::error::attachingInProgress);
        }

        assert(!_attachPromise);
        _attachPromise = Promise<>();

        _channel = std::move(arg_0);
        _channel._input.signal_flow().connect(&Brim<Derived>::onInput, this);

        Future<> res = _attachPromise.future();
        res.then([&](Future<> &f)
        {
            if(f.hasError())
            {
                assert(!_detachPromise);
                assert(_attachPromise);
                _attachPromise = Promise<>(dci::async::PromiseNullInitializer());
                _parser.reset();

                _channel._input.signal_flow().disconnect();
            }
        });

        static_cast<Derived *>(this)->channelAttaching();

        return res;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Derived>
    Future< Channel> Brim<Derived>::detachChannel()
    {
        if(!_channel._input)
        {
            assert(!_channel._output);
            assert(!_detachPromise);
            return make_error_code(streamer::error::alreadyDetached);
        }

        if(_detachPromise)
        {
            return make_error_code(streamer::error::detachingInProgress);
        }

        _detachPromise = Promise<Channel>();

        Future<Channel> res = _detachPromise.future();

        static_cast<Derived *>(this)->channelDetaching();

        return res;
    }

    namespace
    {
        struct LocalStreamSink
            : public dci::couple::serialize::Stream<Settings, LocalStreamSink>
        {
            LocalStreamSink()
                : dci::couple::serialize::Stream<Settings, LocalStreamSink>(*this)
            {
            }

            template <class T> void writePod(const T &v)
            {
                writeBinary(&v, sizeof(v));
            }

            void writeBinary(const void *data, std::size_t size)
            {
                assert(_size + size <= sizeof(_data));
                memcpy(_data+_size, data, size);
                _size += size;
            }

        public:
            char _data[32];
            std::size_t _size {0};
        };

    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Derived>
    template <class Message>
    Future< > Brim<Derived>::output(Message &&message)
    {
        assert(_channel._output);

        Bytes data;
        static const std::size_t sizePlaceReserved = 4;
        data.append("\0\0\0\0", sizePlaceReserved);//зарезервировать место под размер

        StreamSink stream(data);
        stream << Message::_type;
        stream << std::move(message);

        //write body size to data
        std::uint32_t size = data.size() - sizePlaceReserved;

        LocalStreamSink localStream;
        localStream << size;
        assert(localStream._size <= sizePlaceReserved);
        if(localStream._size < sizePlaceReserved)
        {
            data.detachFirst(sizePlaceReserved - localStream._size);
        }
        for(std::size_t i(0); i<localStream._size; ++i)
        {
            data[i] = localStream._data[i];
        }

        return _channel._output.flow(std::move(data));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Derived>
    void Brim<Derived>::attachChannelComplete(std::error_code &&ec)
    {
        assert(!_detachPromise);
        assert(_attachPromise);
        if(ec)
        {
            _attachPromise.resolveError(std::move(ec));
        }
        else
        {
            _attachPromise.resolveValue();
        }
        _attachPromise = Promise<>(dci::async::PromiseNullInitializer());
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Derived>
    void Brim<Derived>::detachChannelComplete(std::error_code &&ec)
    {
        assert(!_attachPromise);
        assert(_detachPromise);

        if(ec)
        {
            _detachPromise.resolveError(std::move(ec));
        }
        else
        {
            _channel._input.signal_flow().disconnect();
            _detachPromise.resolveValue(std::move(_channel));
        }

        _detachPromise = Promise<Channel>(dci::async::PromiseNullInitializer());
        _parser.reset();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Derived>
    Future<> Brim<Derived>::onInput(Bytes &&data)
    {
        std::error_code ec = _parser.process(std::forward<Bytes>(data), [this](auto &&message)
        {
            static_cast<Derived *>(this)->input(std::move(message));
        });

        if(ec && ::streamer::error::lowData != ec)
        {
            static_cast<Derived *>(this)->error(std::error_code(ec));
            return Future<>(std::error_code(ec));
        }

        return Future<>();
    }

}
