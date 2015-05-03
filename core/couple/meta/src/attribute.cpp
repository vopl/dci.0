#include <dci/couple/meta/attribute.hpp>
#include "impl/attribute.hpp"
#include <dci/couple/meta/type.hpp>
#include "impl/type.hpp"

namespace dci { namespace couple { namespace meta
{
    const Type *Attribute::type() const
    {
        return himpl::impl2Face<Type>(impl().type());
    }

}}}
