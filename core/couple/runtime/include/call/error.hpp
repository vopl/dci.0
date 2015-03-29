#pragma once

#include <system_error>

#include <cassert>

namespace dci { namespace couple { namespace runtime { namespace call
{
    class Error
        : public std::error_code
    {

    };

}}}}
