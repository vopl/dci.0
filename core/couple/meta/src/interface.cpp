#include <dci/couple/meta/interface.hpp>
#include "impl/interface.hpp"
#include <cassert>

namespace dci { namespace couple { namespace meta
{
    const std::vector<const Method *> &Interface::methods() const
    {
        return Compound<Method>::elements();
    }

    bool Interface::primary() const
    {
        return impl().primary();
    }

    const Interface *Interface::opposite() const
    {
        return himpl::impl2Face<Interface>(impl().opposite());
    }

}}}
