#pragma once

#include "api.hpp"
#include "sign.hpp"
#include <system_error>
#include <map>
#include <string>

namespace dci { namespace couple { namespace runtime
{
    APIDCI_COUPLE_RUNTIME const std::error_category &errcCategory(const Sign &sign, const char *name, const std::map<int, const std::string> &errors);

}}}
