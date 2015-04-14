#include "container1.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    Container1::Container1(TypeConcrete concrete)
        : Type(concrete)
    {

    }

    Container1::~Container1()
    {

    }

    void Container1::setElementType(Type *v)
    {
        _elementType = v;
    }

    const Type *Container1::elementType() const
    {
        return _elementType;
    }

}}}}
