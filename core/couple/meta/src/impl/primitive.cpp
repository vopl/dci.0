#include "primitive.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    Primitive::Primitive()
        : Type(TypeConcrete::primitive)
    {
    }

    Primitive::~Primitive()
    {

    }

    void Primitive::setKind(PrimitiveKind kind)
    {
        _kind = kind;
    }

    PrimitiveKind Primitive::kind() const
    {
        return _kind;
    }

}}}}
