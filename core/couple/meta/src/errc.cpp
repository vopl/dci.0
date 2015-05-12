#include <dci/couple/meta/errc.hpp>
#include "impl/errc.hpp"
namespace dci { namespace couple { namespace meta
{
    const std::vector<const ErrcValue *> &Errc::values() const
    {
        return Compound<ErrcValue>::elements();
    }

}}}
