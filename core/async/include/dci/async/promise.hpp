#pragma once

#include "details/futureState.hpp"

#include <cassert>
#include <cstdlib>

namespace dci { namespace async
{
    template <class E, class... T> class Future;

    struct PromiseNullInitializer{};

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    class Promise
        : private dci::mm::SharedInstance<details::FutureState<E, T...>>
    {
        using StateInstance = dci::mm::SharedInstance<details::FutureState<E, T...>>;

        Promise(const Promise &other) = delete;
        Promise &operator=(const Promise &other) = delete;

    public:
        using Future = dci::async::Future<E, T...>;


    public:
        explicit Promise(PromiseNullInitializer);
        Promise();
        Promise(Promise &&other);
        Promise &operator=(Promise &&other);

        ~Promise();

        Future future();

        bool resolved() const;
        void resolveValue(T&&... val);
        void resolveValue(::std::tuple<T...> &&val);
        void resolveValue(const ::std::tuple<T...> &val);
        void resolveError(E&& err);
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    Promise<E, T...>::Promise(PromiseNullInitializer)
        : StateInstance(dci::mm::SharedInstanceNullInitializer())
    {
    }

    template <class E, class... T>
    Promise<E, T...>::Promise()
        : StateInstance()
    {
    }

    template <class E, class... T>
    Promise<E, T...>::Promise(Promise &&other)
        : StateInstance(std::forward<StateInstance>(other))
    {
    }

    template <class E, class... T>
    Promise<E, T...> &Promise<E, T...>::operator=(Promise &&other)
    {
        this->StateInstance::operator=( std::forward<StateInstance>(other));
        return *this;
    }

    template <class E, class... T>
    Promise<E, T...>::~Promise()
    {
        if(this->counter()>1 && !this->instance().resolved())
        {
            assert(!"unsetted promise destroyed while futures exists");
            std::abort();
        }
    }

    template <class E, class... T>
    Future<E, T...> Promise<E, T...>::future()
    {
        return Future(*this);
    }

    template <class E, class... T>
    bool Promise<E, T...>::resolved() const
    {
        return this->instance().resolved();
    }

    template <class E, class... T>
    void Promise<E, T...>::resolveValue(T&&... val)
    {
        this->instance().resolveValue(std::forward<T>(val)...);
    }

    template <class E, class... T>
    void Promise<E, T...>::resolveValue(::std::tuple<T...> &&val)
    {
        this->instance().resolveValue(std::move(val));
    }

    template <class E, class... T>
    void Promise<E, T...>::resolveValue(const ::std::tuple<T...> &val)
    {
        this->instance().resolveValue(val);
    }

    template <class E, class... T>
    void Promise<E, T...>::resolveError(E&& err)
    {
        this->instance().resolveError(std::forward<E>(err));
    }

}}

#include "future.hpp"
