#pragma once

#include "api.hpp"
#include <system_error>

namespace dci { namespace poll
{
    enum class err_general
    {
        unknown = 1,
        already_initialized,
        already_started,
        no_engine_available,
        not_initialized,
        not_stopped,
        bad_descriptor,
    };

    APIDCI_POLL const std::error_category& err_general_category();
    APIDCI_POLL std::error_code make_error_code(err_general e);
}}

namespace std
{
    template<>
    struct is_error_code_enum<dci::poll::err_general>
        : public true_type { };
}

