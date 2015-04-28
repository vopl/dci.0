#include "primitive.hpp"
#include <dci/couple/runtime/signBuilder.hpp>

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

    void Primitive::makeSign()
    {
        runtime::SignBuilder s;

        s.add("primitive");
        s.add((std::uint32_t)_kind);

        setSign(s.finish());
    }

}}}}
