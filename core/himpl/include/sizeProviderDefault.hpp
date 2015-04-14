#pragma once

#include "sizeProvider.hpp"

namespace dci { namespace himpl
{
    template <class T>
    struct sizeProvider
    {
        static const std::size_t _value = 0;
        static const std::size_t _align = 1;
    };

}}
