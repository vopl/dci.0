#pragma once

#include "api.hpp"
#include <system_error>

namespace dci { namespace couple { namespace runtime
{
    enum class err_general
    {
        call_not_connected = 1,
    };

    APIDCI_COUPLE_RUNTIME const std::error_category& err_general_category();
    APIDCI_COUPLE_RUNTIME std::error_code make_error_code(err_general e);

}}}

namespace std
{
    template<>
    struct is_error_code_enum<dci::couple::runtime::err_general>
        : public true_type { };

}
