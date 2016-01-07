#pragma once

#include <dci/couple/runtime.hpp>
#include <dci/utils/switchExpander.hpp>
#include "../protocol/message.hpp"
#include "streamer.hpp"
#include "../serialize.hpp"

namespace spares { namespace protocol
{
    using namespace dci::couple::runtime;


    template <class... TypedMessages>
    class Parser
    {
    public:
        Parser();
        ~Parser();

        void reset();

        template <class F>
        std::error_code process(Bytes &&input, F &&resultReceiver);

        const std::error_code &lastError() const;

    private:
        template <class Message, class F>
        std::error_code flush(F &&resultReceiver);

    private:
        Bytes           _accumuler;
        std::size_t     _nextBodyLength {0};

        std::error_code _lastError;

    };

    template <class... TypedMessages>
    Parser<TypedMessages...>::Parser()
    {
    }

    template <class... TypedMessages>
    Parser<TypedMessages...>::~Parser()
    {
    }

    template <class... TypedMessages>
    void Parser<TypedMessages...>::reset()
    {
        _accumuler.clear();
        _nextBodyLength = 0;
        _lastError = std::error_code();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class... TypedMessages>
    template <class F>
    std::error_code Parser<TypedMessages...>::process(Bytes &&input, F &&resultReceiver)
    {
        if(_lastError)
        {
            if(::streamer::error::lowData != _lastError)
            {
                return _lastError;
            }
            _lastError = std::error_code();
        }

        _accumuler.append(std::move(input));


        while(!_accumuler.empty())
        {
            if(!_nextBodyLength)
            {
                //start new chunk

                std::uint32_t bodyLength;

                StreamSource stream(_accumuler, _accumuler.size());
                stream >> bodyLength;//TODO: контроль длины отсутствует, если данных мало
                if(!bodyLength)
                {
                    _lastError = make_error_code(::streamer::error::malformedData);
                    return _lastError;
                }

                _nextBodyLength = bodyLength;
            }

            //continue read body
            auto accumulerSizeBefore = _accumuler.size();
            if(_accumuler.size() >= _nextBodyLength)
            {
                StreamSource stream(_accumuler, _nextBodyLength);
                MessageType messageType=0;
                stream >> messageType;//TODO: контроль длины отсутствует, если данных мало

                dci::utils::SwitchExpander<
                        std::integer_sequence<MessageType, TypedMessages::_type...>,
                        std::tuple<TypedMessages...>
                >::exec(
                            messageType,
                            [&](auto *vholder){
                                std::decay_t<decltype(*vholder)> message;
                                stream >> message;
                                resultReceiver(std::move(message));
                            },
                            [&](MessageType /*messageType*/){
                                assert(!"unknown message type");//TODO: отреагировать на неизвестный тип сообщения
                            }
                );

                (void)accumulerSizeBefore;
                assert(_accumuler.size() + _nextBodyLength == accumulerSizeBefore);
                _nextBodyLength = 0;

            }
            else
            {
                break;
            }


        }
        (void)resultReceiver;
        assert(0);

        return _lastError;
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class... TypedMessages>
    const std::error_code &Parser<TypedMessages...>::lastError() const
    {
        return _lastError;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class... TypedMessages>
    template <class Message, class F>
    std::error_code Parser<TypedMessages...>::flush(F &&resultReceiver)
    {
        Message message;
        assert(0);

        StreamSource stream(_accumuler, _nextBodyLength);

        stream >> message;

        resultReceiver(std::move(message));

        return std::error_code();
    }

}}
