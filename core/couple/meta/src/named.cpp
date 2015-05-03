#include <dci/couple/meta/named.hpp>
#include "impl/named.hpp"

namespace dci { namespace couple { namespace meta
{
    const std::string &Named::name() const
    {
        return impl().name();
    }

}}}
