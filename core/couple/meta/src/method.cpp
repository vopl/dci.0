#include <method.hpp>
#include "impl/method.hpp"

namespace dci { namespace couple { namespace meta
{
    CallDirection Method::direction() const
    {
        return impl().direction();
    }

    bool Method::nowait() const
    {
        return impl().nowait();
    }

    const Type *Method::resultType() const
    {
        return himpl::impl2Face<Type>(impl().resultType());
    }

    const std::vector<Attribute *> &Method::attributes() const
    {
        return Compound<Attribute>::elements();
    }

}}}
