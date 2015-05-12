#include <dci/couple/meta/errcValue.hpp>
#include "impl/errcValue.hpp"

namespace dci { namespace couple { namespace meta
{
    const std::string &ErrcValue::description() const
    {
        return impl().description();
    }
}}}
