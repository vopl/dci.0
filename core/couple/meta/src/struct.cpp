#include <dci/couple/meta/struct.hpp>
#include "impl/struct.hpp"

namespace dci { namespace couple { namespace meta
{
    const std::vector<const Attribute *> &Struct::fields() const
    {
        return Compound<Attribute>::elements();
    }

}}}
