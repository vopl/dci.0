#pragma once

#include "sign.hpp"

#include <cstddef>
#include <string>

namespace dci { namespace couple { namespace runtime
{
    template <class Tag>
    class Identifier
        : public Sign
    {
    public:
        using Sign::Sign;
    };

}}}
