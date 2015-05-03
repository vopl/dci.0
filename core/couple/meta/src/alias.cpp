#include <dci/couple/meta/alias.hpp>
#include "impl/alias.hpp"

namespace dci { namespace couple { namespace meta
{
    const Type *Alias::type() const
    {
        return himpl::impl2Face<Type>(impl().type());
    }

}}}
