#include "container2.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    Container2::Container2(TypeConcrete concrete)
        : Type(concrete)
    {

    }

    Container2::~Container2()
    {

    }

    void Container2::setElementType1(Type *v)
    {
        _elementType1 = v;
    }

    void Container2::setElementType2(Type *v)
    {
        _elementType2 = v;
    }

    const Type *Container2::elementType1() const
    {
        return _elementType1;
    }

    const Type *Container2::elementType2() const
    {
        return _elementType2;
    }

}}}}
