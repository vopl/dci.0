#include <attribute.hpp>
#include "impl/attribute.hpp"
#include <type.hpp>
#include "impl/type.hpp"

namespace dci { namespace couple { namespace meta
{
    const Type *Attribute::type() const
    {
        return himpl::impl2Face<Type>(impl().type());
    }

}}}
