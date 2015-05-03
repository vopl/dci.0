#include <dci/couple/meta/inheritable.hpp>
#include "impl/inheritable.hpp"

namespace dci { namespace couple { namespace meta
{
    template <class B>
    const std::vector<const B *> &Inheritable<B>::bases() const
    {
        return this->impl().bases();
    }

    template class Inheritable<Struct>;
    template class Inheritable<Iface>;

}}}
