#pragma once

#include "type.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Container1
        : public Type
    {
    public:
        Container1(TypeConcrete concrete);
        ~Container1();

        void setElementType(Type *v);
        const Type *elementType() const;

    private:
        Type *_elementType{nullptr};
    };

}}}}
