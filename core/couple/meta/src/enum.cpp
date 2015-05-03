#include <dci/couple/meta/enum.hpp>
#include "impl/enum.hpp"
namespace dci { namespace couple { namespace meta
{
    const std::vector<const EnumValue *> &Enum::values() const
    {
        return Compound<EnumValue>::elements();
    }

}}}
