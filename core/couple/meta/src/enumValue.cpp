#include <dci/couple/meta/enumValue.hpp>
#include "impl/enumValue.hpp"

namespace dci { namespace couple { namespace meta
{
    const std::string &EnumValue::value() const
    {
        return impl().value();
    }

}}}
