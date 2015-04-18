#include <variant.hpp>
#include "impl/variant.hpp"

namespace dci { namespace couple { namespace meta
{
    const std::vector<const Attribute *> &Variant::fields() const
    {
        return Compound<Attribute>::elements();
    }

}}}
