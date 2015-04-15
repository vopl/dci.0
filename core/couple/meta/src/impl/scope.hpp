#pragma once

#include "named.hpp"
#include "scoped.hpp"
#include "signed.hpp"
#include "compound.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{

    class Struct;
    class Variant;
    class Enum;
    class Alias;
    class Iface;

    class Scope
        : public Named
        , public Scoped
        , public Signed
        , public Compound<Struct>
        , public Compound<Variant>
        , public Compound<Enum>
        , public Compound<Alias>
        , public Compound<Iface>
        , public Compound<Scope>
    {
    public:
        Scope();
        ~Scope();

        using Compound<Struct>::add;
        using Compound<Variant>::add;
        using Compound<Enum>::add;
        using Compound<Alias>::add;
        using Compound<Iface>::add;
        using Compound<Scope>::add;

        void makeSign() override;
    };

}}}}
