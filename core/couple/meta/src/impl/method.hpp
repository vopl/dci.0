#pragma once

#include "named.hpp"
#include "compound.hpp"
#include "attribute.hpp"
#include "type.hpp"
#include <dci/couple/meta/method.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Method
        : public Named
        , public Signed
        , public Compound<Attribute>
        , public Compound<Type>
    {
    public:
        Method();
        ~Method();

        using Compound<Attribute>::add;
        using Compound<Type>::add;

        void setDirection(CallDirection v);
        CallDirection direction() const;

        void setNoreply(bool v);
        bool noreply() const;

        void makeSign();

    private:
        CallDirection   _direction{CallDirection::in};
        bool _noreply{false};
    };

}}}}
