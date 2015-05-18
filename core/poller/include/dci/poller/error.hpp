#pragma once

#include <system_error>

namespace dci { namespace poller { namespace error
{
    enum class general
    {
        unknown = 1,
        already_initialized,
        no_engine_available,
        not_initialized,
        bad_descriptor,
    };

    const std::error_category& general_category();
}}}

namespace std
{
    template<>
    struct is_error_code_enum<dci::poller::error::general>
        : public true_type { };
}

namespace dci { namespace poller
{
    std::error_code make_error_code(error::general e);
}}
