#pragma once

#include <system_error>

namespace dci { namespace poll
{
    std::error_code initialize();
    std::error_code run();
    std::error_code stop();
    std::error_code deinitialize();
}}
