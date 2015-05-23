#pragma once

#include <system_error>

namespace dci { namespace poll { namespace error
{
    enum class general
    {
        unknown = 1,
        already_initialized,
        already_started,
        no_engine_available,
        not_initialized,
        not_stopped,
        bad_descriptor,
    };

    const std::error_category& general_category();
}}}

namespace std
{
    template<>
    struct is_error_code_enum<dci::poll::error::general>
        : public true_type { };
}

namespace dci { namespace poll
{
    std::error_code make_error_code(error::general e);
}}
