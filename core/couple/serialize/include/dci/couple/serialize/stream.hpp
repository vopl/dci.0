#pragma once

#include "details/processor.hpp"
#include <type_traits>

namespace dci { namespace couple { namespace serialize
{

    template <class Settings, class SinkSource>
    class Stream
    {
    private:
        using Processor = details::Processor<Settings, SinkSource>;

    private:

        template< class, class = void >
        struct hasLoader : std::false_type { };

        template< class Value >
        struct hasLoader<Value, decltype(std::declval<Processor>().load(std::declval<Value&>()))> : std::true_type { };

        template< class, class = void >
        struct hasSaver : std::false_type { };

        template< class Value >
        struct hasSaver<Value, decltype(std::declval<Processor>().save(std::declval<Value>()))> : std::true_type { };

    public:
        Stream(SinkSource &sinkSource);
        ~Stream();

        template <class Value>
        std::enable_if_t<hasLoader<Value>::value, Stream &> operator>>(Value &value);

        template <class Value>
        std::enable_if_t<hasSaver<Value>::value, Stream &> operator<<(Value &&value);


    private:
        Processor _processor;

    };


    template <class Settings, class SinkSource>
    Stream<Settings, SinkSource>::Stream(SinkSource &sinkSource)
        : _processor(sinkSource)
    {
    }

    template <class Settings, class SinkSource>
    Stream<Settings, SinkSource>::~Stream()
    {
    }

    template <class Settings, class SinkSource>
    template <class Value>

    std::enable_if_t<Stream<Settings, SinkSource>::template hasLoader<Value>::value, Stream<Settings, SinkSource> &>
        Stream<Settings, SinkSource>::operator>>(Value &value)
    {
        _processor.load(value);
        return *this;
    }

    template <class Settings, class SinkSource>
    template <class Value>
    std::enable_if_t<Stream<Settings, SinkSource>::template hasSaver<Value>::value, Stream<Settings, SinkSource> &>
        Stream<Settings, SinkSource>::operator<<(Value &&value)
    {
        _processor.save(std::forward<Value>(value));
        return *this;
    }

}}}
