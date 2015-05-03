#include <dci/couple/meta/iface.hpp>
#include "impl/iface.hpp"
#include <cassert>

namespace dci { namespace couple { namespace meta
{
    const std::vector<const Method *> &Iface::methods() const
    {
        return Compound<Method>::elements();
    }

    bool Iface::forward() const
    {
        assert(0);
    }

}}}
