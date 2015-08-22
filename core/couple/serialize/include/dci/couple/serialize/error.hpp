#pragma once

#include "api.hpp"
#include <system_error>

namespace dci { namespace couple { namespace serialize
{
    enum class err_general
    {
        bad_input = 2,
        quote_exhausted = 3,
    };

    APIDCI_COUPLE_SERIALIZE const std::error_category& err_general_category();
    APIDCI_COUPLE_SERIALIZE std::error_code make_error_code(err_general e);

}}}

namespace std
{
    template<> struct is_error_code_enum<dci::couple::serialize::err_general> : public true_type { };
}
