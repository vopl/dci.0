#pragma once

#include "layoutProvider.hpp"

namespace dci { namespace himpl
{
    template <class T>
    struct LayoutProvider
    {
        static const std::size_t _size = 0;
        static const std::size_t _align = 1;
        static const bool _polymorphic = false;
    };

}}
