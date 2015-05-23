#pragma once

#include <system_error>

namespace dci { namespace io
{
    enum class err_general
    {
        already_runned = 1,
        not_runned,
        invalid_argument,
        secondary_listen,
        no_listen,
        not_implemented,
        unknown,
    };

    enum class err_stream
    {
        not_connected = 1,
        closed,
    };

    const std::error_category& err_general_category();
    const std::error_category& err_stream_category();

    std::error_code make_error_code(err_general e);
    std::error_code make_error_code(err_stream e);

}}

namespace std
{
    template<>
    struct is_error_code_enum<dci::io::err_general>
        : public true_type { };

    template<>
    struct is_error_code_enum<dci::io::err_stream>
        : public true_type { };
}
