#include <sign.hpp>
#include "impl/sign.hpp"

namespace dci { namespace couple { namespace meta
{
    std::string Sign::string(std::size_t chars) const
    {
        return impl().string(chars);
    }

}}}
