#pragma once

#include "../waiter.hpp"
#include "../waitable.hpp"

#include <cstdint>
#include <type_traits>

namespace dci { namespace async { namespace details
{
    ////////////////////////////////////////////////////////////////////////////////////
    inline std::size_t waitablesAmount()
    {
        return 0;
    }

    template <class First, class... Waitables>
    std::size_t waitablesAmount(First &first, Waitables&... waitables)
    {
        (void)first;
        return 1 + waitablesAmount(waitables...);
    }

    template <template <class...> class Container, class First, class... Waitables, class... ContainerExtraArgs>
    std::size_t waitablesAmount(Container<First, ContainerExtraArgs...> &cnt, Waitables&... waitables)
    {
        return cnt.size() + waitablesAmount(waitables...);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    enum class WaitFor
    {
        any,
        all,
        each
    };

    ////////////////////////////////////////////////////////////////////////////////////
    template <WaitFor waitFor>
    void collectWaitables(WWLink *)
    {
    }

    template <WaitFor waitFor, class First, class... Waitables>
    void collectWaitables(WWLink *links, First &first, Waitables&... waitables)
    {
        links->_waitable = himpl::face2Impl(static_cast<Waitable *>(&first));
        collectWaitables<waitFor>(links+1, waitables...);
    }

    template <WaitFor waitFor, template <class...> class Container, class First, class... Waitables, class... ContainerExtraArgs>
    void collectWaitables(WWLink *links, Container<First, ContainerExtraArgs...> &cnt, Waitables&... waitables)
    {
        for(First &w : cnt)
        {
            links->_waitable = himpl::face2Impl(static_cast<Waitable *>(&w));
            links++;
        }

        return collectWaitables<waitFor>(links, waitables...);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    template <WaitFor waitFor>
    std::size_t executeWaiter(Waiter &waiter)
    {
        switch(waitFor)
        {
        case WaitFor::any:  return waiter.any();
        case WaitFor::all:  waiter.all(); return 0;
        case WaitFor::each: waiter.each(); return 0;
        }

        assert(!"never here");
    }

    template <WaitFor waitFor, class... Waitables>
    std::size_t wait(Waitables&... waitables)
    {
        const std::size_t amount = waitablesAmount(waitables...);

        WWLink *links = (WWLink *)alloca(sizeof(WWLink) * amount);
        collectWaitables<waitFor>(links, waitables...);

        Waiter waiter(links, amount);

        return executeWaiter<waitFor>(waiter);
    }

}}}
