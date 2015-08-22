#pragma once

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
    struct ValueTraits
    {
        static const ValueKind _kind = ValueKind::undefined;
    };
}}}
