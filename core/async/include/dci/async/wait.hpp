#pragma once

#include <cstdint>

namespace dci { namespace async
{
    template <class... Waitable>
    std::size_t waitAny(Waitable&... waitables);

    template <class... Waitable>
    void waitAll(Waitable&... waitables);

    template <class... Waitable>
    void waitEach(Waitable&... waitables);
}}

#include "details/wait.hpp"

namespace dci { namespace async
{
    template <class... Waitable>
    std::size_t waitAny(Waitable&... waitables)
    {
        return details::wait<details::WaitFor::any>(waitables...);
    }

    template <class... Waitable>
    void waitAll(Waitable&... waitables)
    {
        details::wait<details::WaitFor::all>(waitables...);
    }

    template <class... Waitable>
    void waitEach(Waitable&... waitables)
    {
        details::wait<details::WaitFor::each>(waitables...);
    }

}}
