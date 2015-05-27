#pragma once

#include "api.hpp"
#include <vector>
#include <string>

namespace dci { namespace couple { namespace parser
{
    struct APIDCI_COUPLE_PARSER Config
    {
        std::vector<std::string>    _includeDirectories;
        bool                        _printSourceFiles = false;

    };
}}}
