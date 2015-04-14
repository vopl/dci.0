#include "type.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    Type::Type(TypeConcrete concrete)
        : _concrete (concrete)
    {
    }

    TypeConcrete Type::concrete() const
    {
        return _concrete;
    }

}}}}
