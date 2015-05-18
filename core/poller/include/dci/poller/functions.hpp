#pragma once

#include <system_error>

namespace dci { namespace poller
{
    std::error_code initialize();
    std::error_code execute(std::int32_t timeoutms=-1);
    std::error_code interrupt();
    std::error_code deinitialize();
}}
