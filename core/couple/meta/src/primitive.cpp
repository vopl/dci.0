#include <dci/couple/meta/primitive.hpp>
#include "impl/primitive.hpp"

namespace dci { namespace couple { namespace meta
{
    PrimitiveKind Primitive::kind() const
    {
        return impl().kind();
    }

}}}
