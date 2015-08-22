#pragma once

#include "api.hpp"
#include <system_error>

namespace dci { namespace couple { namespace runtime
{
    enum class err_general
    {
        call_not_connected = 1,
    };

    enum class err_system
    {
        unknown = 1,

#define X_DCI_COUPLE_RUNTIME_ERR_SYSTEM(v, d) v,
#include <dci/couple/runtime/err_system.x>
#undef X_DCI_COUPLE_RUNTIME_ERR_SYSTEM

    };

    APIDCI_COUPLE_RUNTIME const std::error_category& err_general_category();
    APIDCI_COUPLE_RUNTIME std::error_code make_error_code(err_general e);

    APIDCI_COUPLE_RUNTIME const std::error_category& err_system_category();
    APIDCI_COUPLE_RUNTIME std::error_code make_error_code(err_system e);
    APIDCI_COUPLE_RUNTIME std::error_code systemError();
    APIDCI_COUPLE_RUNTIME std::error_code systemError(const std::error_code &ec);

}}}

namespace std
{
    template<> struct is_error_code_enum<dci::couple::runtime::err_general> : public true_type { };
    template<> struct is_error_code_enum<dci::couple::runtime::err_system> : public true_type { };

}
