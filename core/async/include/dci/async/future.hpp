#pragma once

#include "details/future.hpp"
#include <functional>

namespace dci { namespace async
{
    template <class E, class... T> class Promise;

    struct FutureNullInitializer{};

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    namespace details
    {
        template <class F, class... ET>
        struct FutureOperators;

        template <class F, class E, class... T>
        struct FutureOperators<F, E, T...>
        {
            operator E &&()
            {
                return static_cast<F*>(this)->detachError();
            }
        };

        template <class F, class E, class T0>
        struct FutureOperators<F, E, T0>
        {
            operator E &&()
            {
                return static_cast<F*>(this)->detachError();
            }

            operator T0 &&()
            {
                return static_cast<F*>(this)->template detachValue<0>();
            }

        };

        template <class F, class E, class T0, class T1>
        struct FutureOperators<F, E, T0, T1>
        {
            operator E &&()
            {
                return static_cast<F*>(this)->detachError();
            }

            operator T0 &&()
            {
                return static_cast<F*>(this)->template detachValue<0>();
            }

            operator T1 &&()
            {
                return static_cast<F*>(this)->template detachValue<1>();
            }
        };
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    class Future
        : public details::FutureOperators<Future<E, T...>, E, T...>
    {
        friend class Promise<E, T...>;
        friend class details::future::Engine<E, T...>;
        using Engine = details::future::Engine<E, T...>;
        explicit Future(const Engine &engine);

    public:
        using Promise = dci::async::Promise<E, T...>;

    public:
        explicit Future(FutureNullInitializer);
        Future(E && err);
        Future(T &&... vals);
        Future(const Future &other);
        Future(Future &&other);
        ~Future();

        Future &operator=(const Future &other);
        Future &operator=(Future &&other);

        operator bool() const;

        Waitable *waitable();
        Future &wait();

        bool resolved() const;
        bool hasError();
        bool hasValue();

        const std::tuple<T...> &value();

        template <std::size_t idx>
        const typename std::tuple_element<idx, std::tuple<T...> >::type &value();

        std::tuple<T...> &&detachValue();

        template <std::size_t idx>
        typename std::tuple_element<idx, std::tuple<T...> >::type &&detachValue();

        const E &error();
        E &&detachError();

        template <class F, class... Args>
        void then(F &&, Args &&...);

        template <class Et, class... Tt, class F, class... Args>
        Future<Et, Tt...> thenTransform(F &&, Args &&...);

    private:
        Engine _engine;
    };

    template <class E, class... T>
    Future<E, T...>::Future(FutureNullInitializer)
        : _engine(details::future::EngineNullInitializer())
    {
    }

    template <class E, class... T>
    Future<E, T...>::Future(E && err)
        : _engine()
    {
        _engine.resolveError(std::forward<E>(err));
    }

    template <class E, class... T>
    Future<E, T...>::Future(T &&... vals)
        : _engine()
    {
        _engine.resolveValue(std::forward<T>(vals)...);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    Future<E, T...>::Future(const Engine &engine)
        : _engine(engine)
    {
    }

    template <class E, class... T>
    Future<E, T...>::Future(const Future &other)
        : _engine(other._engine)
    {
    }

    template <class E, class... T>
    Future<E, T...>::Future(Future &&other)
        : _engine(std::move(other._engine))
    {
    }

    template <class E, class... T>
    Future<E, T...>::~Future()
    {
    }

    template <class E, class... T>
    Future<E, T...> &Future<E, T...>::operator=(const Future &other)
    {
        _engine = other._engine;
        return *this;
    }

    template <class E, class... T>
    Future<E, T...> &Future<E, T...>::operator=(Future &&other)
    {
        _engine = std::move(other._engine);
        return *this;
    }

    template <class E, class... T>
    Future<E, T...>::operator bool() const
    {
        return _engine.operator bool();
    }

    template <class E, class... T>
    Waitable *Future<E, T...>::waitable()
    {
        return _engine.waitable();
    }

    template <class E, class... T>
    Future<E, T...> &Future<E, T...>::wait()
    {
        waitable()->wait();
        return *this;
    }

    template <class E, class... T>
    bool Future<E, T...>::resolved() const
    {
        return _engine.resolved();
    }

    template <class E, class... T>
    bool Future<E, T...>::hasError()
    {
        wait();
        return _engine.resolved2Error();
    }

    template <class E, class... T>
    bool Future<E, T...>::hasValue()
    {
        wait();
        return _engine.resolved2Value();
    }

    template <class E, class... T>
    const std::tuple<T...> &Future<E, T...>::value()
    {
        wait();
        return _engine.value();
    }

    template <class E, class... T>
    template <std::size_t idx>
    const typename std::tuple_element<idx, std::tuple<T...> >::type &Future<E, T...>::value()
    {
        wait();
        return std::get<idx>(_engine.value());
    }

    template <class E, class... T>
    std::tuple<T...> &&Future<E, T...>::detachValue()
    {
        wait();
        return std::move(_engine.value());
    }

    template <class E, class... T>
    template <std::size_t idx>
    typename std::tuple_element<idx, std::tuple<T...> >::type &&Future<E, T...>::detachValue()
    {
        wait();
        return std::get<idx>(std::forward<typename Engine::Value>(_engine.value()));
    }

    template <class E, class... T>
    const E &Future<E, T...>::error()
    {
        wait();
        return _engine.error();
    }

    template <class E, class... T>
    E &&Future<E, T...>::detachError()
    {
        wait();
        return std::move(_engine.error());
    }

    template <class E, class... T>
    template <class F, class... Args>
    void Future<E, T...>::then(F &&f, Args &&... args)
    {
        struct Then
            : details::future::ThenBase<E, T...>
        {
            Then(F &&f, Args &&... args)
                : _call(std::bind(std::forward<F>(f), std::forward<Args>(args)..., std::placeholders::_1))
            {
            }

            void call(Future<E, T...> &ftr) override
            {
                _call(ftr);
            }

            void destroy() override
            {
                this->~Then();
                mm::free<sizeof(Then)>(this);
            }

            std::decay_t<decltype(std::bind(std::forward<F>(std::declval<F>()), std::forward<Args>(std::declval<Args>())..., std::placeholders::_1))> _call;
        } *then = new(mm::alloc<sizeof(Then)>()) Then(std::forward<F>(f), std::forward<Args>(args)...);

        _engine.pushThen(then);
    }

}}

#include "promise.hpp"

namespace dci { namespace async
{
    template <class E, class... T>
    template <class Et, class... Tt, class F, class... Args>
    Future<Et, Tt...> Future<E, T...>::thenTransform(F &&f, Args &&... args)
    {
        struct Then
            : details::future::ThenBase<E, T...>
        {
            Then(F &&f, Args &&... args)
                : _call(std::bind(std::forward<F>(f), std::forward<Args>(args)..., std::placeholders::_1, std::placeholders::_2))
            {
            }

            void call(Future<E, T...> &ftr) override
            {
                _call(ftr, _dstPromise);
            }

            void destroy() override
            {
                this->~Then();
                mm::free<sizeof(Then)>(this);
            }

            std::decay_t<decltype(std::bind(std::forward<F>(std::declval<F>()), std::forward<Args>(std::declval<Args>())..., std::placeholders::_1, std::placeholders::_2))> _call;
            dci::async::Promise<Et, Tt...>  _dstPromise;
        } *then = new(mm::alloc<sizeof(Then)>()) Then(std::forward<F>(f), std::forward<Args>(args)...);

        auto res = then->_dstPromise.future();
        _engine.pushThen(then);
        return res;
    }

}}

