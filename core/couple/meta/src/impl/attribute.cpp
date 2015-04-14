#include "attribute.hpp"

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

}}}}
