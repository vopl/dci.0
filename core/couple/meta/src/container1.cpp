#include <dci/couple/meta/container1.hpp>
#include "impl/container1.hpp"
#include "impl/type.hpp"

namespace dci { namespace couple { namespace meta
{
    const Type *Container1::elementType() const
    {
        return himpl::impl2Face<Type>(impl().elementType());
    }

}}}
