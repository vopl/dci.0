#pragma once

#include <system_error>

namespace dci { namespace site
{
    enum class err_general
    {
        unknown = 1,
        already_started,
        already_stopped,
        starting_in_progress,
        stopping_in_progress,
        modules_directory_absent,
        failed,
        partial_failed,
        not_found,
    };

    enum class err_module
    {
        unknown = 1,
        busy,
        wrong_state,
        unable_load_binary,
        malformed_binary,
        bad_info_file,
    };

    const std::error_category& err_general_category();
    const std::error_category& err_module_category();

    std::error_code make_error_code(err_general e);
    std::error_code make_error_code(err_module e);
}}

namespace std
{
    template<>
    struct is_error_code_enum<dci::site::err_general>
        : public true_type { };

    template<>
    struct is_error_code_enum<dci::site::err_module>
        : public true_type { };
}
