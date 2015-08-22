#pragma once

#include "details/processor.hpp"
#include <type_traits>

namespace dci { namespace couple { namespace serialize
{

    template <class Settings, class Context, class SinkSource>
    class Stream
    {
    private:
        using Processor = details::Processor<Settings, Context, SinkSource>;

    private:
        template< class... >
        using void_t = void;

        template< class, class = void_t<> >
        struct hasLoader : std::false_type { };

        template< class Value >
        struct hasLoader<Value, void_t<decltype(std::declval<Processor>().load(std::declval<Value>()))>> : std::true_type { };

        template< class, class = void_t<> >
        struct hasSaver : std::false_type { };

        template< class Value >
        struct hasSaver<Value, void_t<decltype(std::declval<Processor>().save(std::declval<Value>()))>> : std::true_type { };

    public:
        Stream(Context &ctx, SinkSource &sinkSource);
        ~Stream();

        template <class Value>
        std::enable_if_t<hasLoader<Value>::value, Stream &> operator>>(Value &value);

        template <class Value>
        std::enable_if_t<hasSaver<Value>::value, Stream &> operator<<(Value &&value);


    private:
        Processor _processor;

    };


    template <class Settings, class Context, class SinkSource>
    Stream<Settings, Context, SinkSource>::Stream(Context &ctx, SinkSource &sinkSource)
        : _processor(ctx, sinkSource)
    {
    }

    template <class Settings, class Context, class SinkSource>
    Stream<Settings, Context, SinkSource>::~Stream()
    {
    }

    template <class Settings, class Context, class SinkSource>
    template <class Value>

    std::enable_if_t<Stream<Settings, Context, SinkSource>::template hasLoader<Value>::value, Stream<Settings, Context, SinkSource> &>
        Stream<Settings, Context, SinkSource>::operator>>(Value &value)
    {
        _processor.load(value);
        return *this;
    }

    template <class Settings, class Context, class SinkSource>
    template <class Value>
    std::enable_if_t<Stream<Settings, Context, SinkSource>::template hasSaver<Value>::value, Stream<Settings, Context, SinkSource> &>
        Stream<Settings, Context, SinkSource>::operator<<(Value &&value)
    {
        _processor.save(std::forward<Value>(value));
        return *this;
    }

}}}
