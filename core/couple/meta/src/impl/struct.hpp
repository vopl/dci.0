#pragma once

#include "type.hpp"
#include "scope.hpp"
#include "compound.hpp"
#include "inheritable.hpp"
#include "attribute.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Struct
        : public Type
        , public Scope
        , public Compound<Attribute>
        , public Inheritable<Struct>
    {
    public:
        using Compound<Attribute>::add;
        using Scope::add;

        Struct();
        ~Struct();
    };

}}}}
