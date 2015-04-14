#pragma once

#include "named.hpp"
#include "compound.hpp"
#include "attribute.hpp"
#include "type.hpp"
#include <method.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Method
        : public Named
        , public Compound<Attribute>
    {
    public:
        using Compound<Attribute>::add;

        Method();
        ~Method();

        void setDirection(CallDirection v);
        void setNowait(bool v);
        void setResultType(Type *v);

        CallDirection direction() const;
        bool nowait() const;
        const Type *resultType() const;

    private:
        CallDirection   _direction{CallDirection::in};
        bool            _nowait{false};
        Type            *_resultType{nullptr};

    };

}}}}
