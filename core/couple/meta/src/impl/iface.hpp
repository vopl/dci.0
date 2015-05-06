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
        Iface();
        ~Iface();

        using Compound<Method>::add;
        using Scope::add;

        void makeSign();

        void setPrimary(bool v);
        bool primary() const;

        void setOpposite(Iface *v);
        const Iface *opposite() const;

    private:
        bool    _primary{true};
        Iface * _opposite{nullptr};
    };

}}}}
