#include <dci/couple/meta/container2.hpp>
#include "impl/container2.hpp"
#include "impl/type.hpp"

namespace dci { namespace couple { namespace meta
{
    const Type *Container2::elementType1() const
    {
        return himpl::impl2Face<Type>(impl().elementType1());
    }

    const Type *Container2::elementType2() const
    {
        return himpl::impl2Face<Type>(impl().elementType2());
    }

}}}
