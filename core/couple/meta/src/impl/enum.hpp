#pragma once

#include "type.hpp"
#include "named.hpp"
#include "scoped.hpp"
#include "compound.hpp"
#include "inheritable.hpp"
#include "enumValue.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Enum
        : public Type
        , public Named
        , public Scoped
        , public Compound<EnumValue>
        , public Inheritable<Enum>
    {
    public:
        Enum();
        ~Enum();

        using Compound<EnumValue>::add;
    };

}}}}
