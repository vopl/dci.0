#include "attribute.hpp"
#include <dci/couple/runtime/signBuilder.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    Attribute::Attribute()
    {

    }

    Attribute::~Attribute()
    {

    }

    void Attribute::setType(Type *v)
    {
        _type = v;
    }

    const Type *Attribute::type() const
    {
        return _type;
    }

    void Attribute::makeSign()
    {
        runtime::SignBuilder s;

        s.add("attribute");
        s.add(_type->concreteSign());
        s.add(name());

        setSign(s.finish());
    }

}}}}
