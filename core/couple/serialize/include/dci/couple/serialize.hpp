#pragma once

#include <system_error>

namespace dci { namespace couple { namespace serialize
{

    enum class ValueKind
    {
        undefined,
        variant,
        struct_,
        enum_,
        service,
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Value>
    class ValueTraits
    {
        static const ValueKind _kind = ValueKind::undefined;
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Settings, class Context, class Value, class Sink>
    std::error_code save(Context &context, Value &&value, Sink &sink);

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Settings, class Context, class Source, class Value>
    std::error_code load(Context &context, Source &source, Value &value);



    enum class Endianness
    {
        unknown,
        little,
        big,
        pdp
    };

#if not defined __BYTE_ORDER or not defined __BYTE_ORDER
#   error __BYTE_ORDER must be defined
#endif

    static const Endianness _currentEndianness =
        __LITTLE_ENDIAN == __BYTE_ORDER ?
            Endianness::little :
            __BIG_ENDIAN == __BYTE_ORDER ?
                Endianness::big :
                __PDP_ENDIAN == __BYTE_ORDER ?
                    Endianness::pdp :
                    Endianness::unknown;
    static_assert(Endianness::unknown != _currentEndianness, "");
    static_assert(Endianness::pdp != _currentEndianness, "");






    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    namespace details
    {
        template <class Settings, class Context, class SinkSource> struct Processor;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Settings, class Context, class Value, class Sink>
    std::error_code save(Context &context, Value &&value, Sink &sink)
    {
        try
        {
            details::Processor<Settings, Context, Sink> processor(context, sink);
            processor.save(std::forward<Value>(value));
        }
        catch(const std::system_error &e)
        {
            return e.code();
        }

        return std::error_code();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Settings, class Context, class Source, class Value>
    std::error_code load(Context &context, Source &source, Value &value)
    {
        try
        {
            details::Processor<Settings, Context, Source> processor(context, source);
            processor.load(value);
        }
        catch(const std::system_error &e)
        {
            return e.code();
        }

        return std::error_code();
    }

}}}

#include "serialize/details/processor.hpp"
