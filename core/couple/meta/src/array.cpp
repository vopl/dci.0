#include <dci/couple/meta/array.hpp>
#include "impl/array.hpp"

#include <type_traits>

namespace dci { namespace couple { namespace meta
{
    std::uint32_t Array::size() const
    {
        return impl().size();
    }

}}}
