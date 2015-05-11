#include <dci/couple/meta/method.hpp>
#include "impl/method.hpp"

namespace dci { namespace couple { namespace meta
{
    CallDirection Method::direction() const
    {
        return impl().direction();
    }

    const std::vector<const Attribute *> &Method::query() const
    {
        return Compound<Attribute>::elements();
    }

    const std::vector<const Type *> &Method::reply() const
    {
        return Compound<Type>::elements();
    }

    bool Method::noreply() const
    {
        return impl().noreply();
    }

}}}
