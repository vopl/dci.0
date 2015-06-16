#pragma once

#include "type.hpp"
#include "scope.hpp"
#include "compound.hpp"
#include "inheritable.hpp"
#include "method.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Interface
        : public Type
        , public Scope
        , public Compound<Method>
        , public Inheritable<Interface>
    {
    public:
        Interface();
        ~Interface();

        using Compound<Method>::add;
        using Scope::add;

        void makeSign();

        void setPrimary(bool v);
        bool primary() const;

        void setOpposite(Interface *v);
        const Interface *opposite() const;

    private:
        bool    _primary{true};
        Interface * _opposite{nullptr};
    };

}}}}
