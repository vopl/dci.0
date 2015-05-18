#pragma once

#include <cstdint>

namespace dci { namespace async
{
    template <class... Acquirable>
    std::size_t acquireAny(Acquirable&... acquirables);

    template <class... Acquirable>
    void acquireAll(Acquirable&... acquirables);
}}

#include "details/acquire.hpp"

namespace dci { namespace async
{
    template <class... Acquirable>
    std::size_t acquireAny(Acquirable&... acquirables)
    {
        return details::acquireAny(acquirables...);
    }

    template <class... Acquirable>
    void acquireAll(Acquirable&... acquirables)
    {
        return details::acquireAll(acquirables...);
    }
}}
