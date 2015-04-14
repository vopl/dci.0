#include <compound.hpp>
#include "impl/compound.hpp"
#include <scope.hpp>
#include <type.hpp>

namespace dci { namespace couple { namespace meta
{

    template <class E>
    const std::vector<E *> &Compound<E>::elements() const
    {
        return this->impl().elements();
    }

    template class Compound<Scope>;
    template class Compound<Type>;
}}}

