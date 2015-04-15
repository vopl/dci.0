#pragma once

#include "type.hpp"
#include "scope.hpp"
#include "compound.hpp"
#include "inheritable.hpp"
#include "attribute.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Variant
        : public Type
        , public Scope
        , public Compound<Attribute>
        , public Inheritable<Variant>
    {
    public:
        Variant();
        ~Variant();

        using Compound<Attribute>::add;
        using Scope::add;

        void makeSign() override;
    };

}}}}
