#pragma once

#include "details/future.hpp"

#include <cassert>
#include <cstdlib>

namespace dci { namespace async
{
    template <class E, class... T> class Future;

    struct PromiseNullInitializer{};

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    class Promise
    {
        using Engine = details::future::Engine<E, T...>;

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

        operator bool() const;

        Future future();

        bool resolved() const;
        void resolveValue(T&&... val);
        void resolveValue(::std::tuple<T...> &&val);
        void resolveValue(const ::std::tuple<T...> &val);
        void resolveError(E&& err);

    private:
        Engine _engine;
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    Promise<E, T...>::Promise(PromiseNullInitializer)
        : _engine(details::future::EngineNullInitializer())
    {
    }

    template <class E, class... T>
    Promise<E, T...>::Promise()
        : _engine()
    {
    }

    template <class E, class... T>
    Promise<E, T...>::Promise(Promise &&other)
        : _engine(std::move(other._engine))
    {
    }

    template <class E, class... T>
    Promise<E, T...> &Promise<E, T...>::operator=(Promise &&other)
    {
        _engine = std::move(other._engine);
        return *this;
    }

    template <class E, class... T>
    Promise<E, T...>::~Promise()
    {
        if(_engine.counter()>1 && !_engine.resolved())
        {
            assert(!"unsetted promise destroyed while futures exists");
            std::abort();
        }
    }

    template <class E, class... T>
    Promise<E, T...>::operator bool() const
    {
        return _engine.operator bool();
    }

    template <class E, class... T>
    Future<E, T...> Promise<E, T...>::future()
    {
        return Future(_engine);
    }

    template <class E, class... T>
    bool Promise<E, T...>::resolved() const
    {
        return _engine.resolved();
    }

    template <class E, class... T>
    void Promise<E, T...>::resolveValue(T&&... val)
    {
        _engine.resolveValue(std::forward<T>(val)...);
    }

    template <class E, class... T>
    void Promise<E, T...>::resolveValue(::std::tuple<T...> &&val)
    {
        _engine.resolveValue(std::move(val));
    }

    template <class E, class... T>
    void Promise<E, T...>::resolveValue(const ::std::tuple<T...> &val)
    {
        _engine.resolveValue(val);
    }

    template <class E, class... T>
    void Promise<E, T...>::resolveError(E&& err)
    {
        _engine.resolveError(std::forward<E>(err));
    }

}}

#include "future.hpp"
