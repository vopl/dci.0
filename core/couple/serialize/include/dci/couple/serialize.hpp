#pragma once

#include <system_error>

namespace dci { namespace couple { namespace serialize
{

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Value>
    class ValueTraits
    {
        static const bool _defined = false;
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Settings, class Context, class Value, class Sink>
    std::error_code save(Context &context, const Value &value, Sink &sink);

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Settings, class Context, class Source, class Value>
    std::error_code load(Context &context, Source &source, Value &value);












    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    namespace details
    {
        template <class Settings, class Context, class SinkSource> struct Processor;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Settings, class Context, class Value, class Sink>
    std::error_code save(Context &context, const Value &value, Sink &sink)
    {
        try
        {
            details::Processor<Settings, Context, Sink>::save(context, sink, value, 0);
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
            details::Processor<Settings, Context, Source>::load(context, source, value, 0);
        }
        catch(const std::system_error &e)
        {
            return e.code();
        }

        return std::error_code();
    }

}}}

#include "serialize/details/processor.hpp"
