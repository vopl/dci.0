#include <iface.hpp>
#include <cassert>

namespace dci { namespace couple { namespace meta
{
    const std::vector<Method *> &Iface::methods() const
    {
        return Compound<Method>::elements();
    }

    bool Iface::forward() const
    {
        assert(0);
    }

}}}
