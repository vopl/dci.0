#pragma once

#include "api.hpp"
#include <system_error>

namespace dci { namespace poll
{
    APIDCI_POLL std::error_code initialize();
    APIDCI_POLL std::error_code run();
    APIDCI_POLL std::error_code stop();
    APIDCI_POLL std::error_code deinitialize();
}}
