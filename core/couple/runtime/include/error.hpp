#pragma once

#include <system_error>

namespace dci { namespace couple { namespace runtime { namespace error
{
    enum class general
    {
        call_not_connected = 1,
    };

    const std::error_category& general_category();

}}}}

namespace std
{
    template<>
    struct is_error_code_enum<dci::couple::runtime::error::general>
        : public true_type { };

}

namespace dci { namespace couple { namespace runtime
{
    std::error_code make_error_code(error::general e);
}}}
