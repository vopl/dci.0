#include <type.hpp>
#include "impl/type.hpp"

namespace dci { namespace couple { namespace meta
{
    TypeConcrete Type::concrete() const
    {
        return impl().concrete();
    }
}}}
