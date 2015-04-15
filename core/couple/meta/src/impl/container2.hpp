#pragma once

#include "type.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Container2
        : public Type
    {
    public:
        Container2(TypeConcrete concrete);
        ~Container2();

        void setElementType1(Type *v);
        void setElementType2(Type *v);

        const Type *elementType1() const;
        const Type *elementType2() const;

    protected:
        Type *_elementType1{nullptr};
        Type *_elementType2{nullptr};
    };

}}}}
