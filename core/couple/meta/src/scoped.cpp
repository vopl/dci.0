#include <scoped.hpp>
#include "impl/scoped.hpp"

#include <scope.hpp>
#include "impl/scope.hpp"

namespace dci { namespace couple { namespace meta
{
    const Scope *Scoped::scope() const
    {
        return himpl::impl2Face<Scope>(impl().scope());
    }

}}}
