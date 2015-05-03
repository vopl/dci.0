#pragma once

#include "../mutex.hpp"
#include "../event.hpp"
#include "../future.hpp"

#include <cstdint>
#include <tuple>

namespace dci { namespace async { namespace impl
{
    class Syncronizer;
    using SyncronizerPtr = Syncronizer *;
}}}

namespace dci { namespace async { namespace details
{

    ////////////////////////////////////////////////////////////////////////////////////
    impl::SyncronizerPtr syncronizer(Mutex &v);
    impl::SyncronizerPtr syncronizer(Event &v);

    template <class... TT>
    impl::SyncronizerPtr syncronizer(Future<TT...> &v)
    {
        return syncronizer(FutureStateAccessor::exec(v)._readyEvent);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    std::size_t acquireAny(impl::SyncronizerPtr *syncronizers, std::size_t amount);
    void acquireAll(impl::SyncronizerPtr *syncronizers, std::size_t amount);

    ////////////////////////////////////////////////////////////////////////////////////
    inline std::size_t acquerablesAmount()
    {
        return 0;
    }

    template <class First, class... Acquirable>
    std::size_t acquerablesAmount(First &first, Acquirable&... acquirables)
    {
        (void)first;
        return 1 + acquerablesAmount(acquirables...);
    }

    template <template <class...> class Container, class First, class... Acquirable, class... ContainerExtraArgs>
    std::size_t acquerablesAmount(Container<First, ContainerExtraArgs...> &first, Acquirable&... acquirables)
    {
        return first.size() + acquerablesAmount(acquirables...);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    inline void collect(impl::SyncronizerPtr *){}

    template <class First, class... Acquirable>
    void collect(impl::SyncronizerPtr *syncronizers, First &first, Acquirable&... acquirables)
    {
        *syncronizers = syncronizer(first);
        return collect(syncronizers+1, acquirables...);
    }

    template <template <class...> class Container, class First, class... Acquirable, class... ContainerExtraArgs>
    void collect(impl::SyncronizerPtr *syncronizers, Container<First, ContainerExtraArgs...> &first, Acquirable&... acquirables)
    {
        using Cnt = Container<First, ContainerExtraArgs...>;

        typename Cnt::iterator iter = std::begin(first);
        typename Cnt::iterator end = std::end(first);

        for(; iter!=end; ++iter)
        {
            *syncronizers = syncronizer(*iter);
            syncronizers++;
        }

        return collect(syncronizers, acquirables...);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    template <class... Acquirable>
    std::size_t acquireAny(Acquirable&... acquirables)
    {
        const std::size_t amount = acquerablesAmount(acquirables...);
        impl::SyncronizerPtr syncronizers[amount];
        collect(syncronizers, acquirables...);
        std::size_t res = acquireAny(syncronizers, amount);
        return res;
    }

    template <class... Acquirable>
    void acquireAll(Acquirable&... acquirables)
    {
        const std::size_t amount = acquerablesAmount(acquirables...);
        impl::SyncronizerPtr syncronizers[amount];
        collect(syncronizers, acquirables...);
        acquireAll(syncronizers, amount);
    }
}}}
