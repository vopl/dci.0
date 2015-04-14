#pragma once

#include "type.hpp"
#include "scope.hpp"
#include "compound.hpp"
#include "inheritable.hpp"
#include "method.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Iface
        : public Type
        , public Scope
        , public Compound<Method>
        , public Inheritable<Iface>
    {
    public:
        using Compound<Method>::add;
        using Scope::add;

        Iface();
        ~Iface();

    };

}}}}
