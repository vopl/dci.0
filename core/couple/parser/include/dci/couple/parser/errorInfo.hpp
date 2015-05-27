#pragma once

#include "api.hpp"
#include <string>

namespace dci { namespace couple { namespace parser
{
    class APIDCI_COUPLE_PARSER ErrorInfo
    {
    public:
        ErrorInfo(const std::string &file, int line, int column, const std::string &message);

        std::string toString() const;

    private:
        std::string     _file;
        int             _line;
        int             _column;
        std::string     _message;
    };
}}}
