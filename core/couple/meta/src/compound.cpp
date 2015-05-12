#include <dci/couple/meta/compound.hpp>
#include "impl/compound.hpp"
#include <dci/couple/meta/scope.hpp>
#include <dci/couple/meta/type.hpp>

namespace dci { namespace couple { namespace meta
{
    template <class E>
    const std::vector<const E *> &Compound<E>::elements() const
    {
        return this->impl().elements();
    }

    template class Compound<Type>;
    template class Compound<Struct>;
    template class Compound<Variant>;
    template class Compound<Enum>;
    template class Compound<EnumValue>;
    template class Compound<Errc>;
    template class Compound<ErrcValue>;
    template class Compound<Alias>;
    template class Compound<Iface>;
    template class Compound<Method>;
    template class Compound<Attribute>;
    template class Compound<Scope>;
}}}

