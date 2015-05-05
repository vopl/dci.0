#pragma once

#include "details/futureState.hpp"

namespace dci { namespace async
{
    template <class E, class... T> class Promise;

    struct FutureNullInitializer{};


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    class Future
        : private dci::mm::SharedInstance<details::FutureState<E, T...>>
    {
        friend class Promise<E, T...>;
        friend struct details::FutureStateAccessor;
        using StateInstance = dci::mm::SharedInstance<details::FutureState<E, T...>>;
        explicit Future(const StateInstance &state);

    public:
        using Promise = dci::async::Promise<E, T...>;

    public:
        explicit Future(FutureNullInitializer);
        explicit Future(E && err);
        explicit Future(T &&... vals);
        Future(const Future &other);
        Future(Future &&other);
        ~Future();

        Future &operator=(const Future &other);
        Future &operator=(Future &&other);

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

        template <class F>
        void then(F &&);

        template <class Et, class... Tt, class F>
        Future<Et, Tt...> thenTransform(F &&);
    };

    template <class E, class... T>
    Future<E, T...>::Future(FutureNullInitializer)
        : StateInstance(dci::mm::SharedInstanceNullInitializer())
    {
    }

    template <class E, class... T>
    Future<E, T...>::Future(E && err)
        : StateInstance()
    {
        this->instance().resolveError(std::forward<E>(err));
    }

    template <class E, class... T>
    Future<E, T...>::Future(T &&... vals)
        : StateInstance()
    {
        this->instance().resolveValue(std::forward<T>(vals)...);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    Future<E, T...>::Future(const StateInstance &state)
        : StateInstance(state)
    {
    }

    template <class E, class... T>
    Future<E, T...>::Future(const Future &other)
        : StateInstance(static_cast<const StateInstance &>(other))
    {
    }

    template <class E, class... T>
    Future<E, T...>::Future(Future &&other)
        : StateInstance(std::forward<StateInstance>(other))
    {

    }

    template <class E, class... T>
    Future<E, T...>::~Future()
    {
    }

    template <class E, class... T>
    Future<E, T...> &Future<E, T...>::operator=(const Future &other)
    {
        this->StateInstance::operator=(other);
        return *this;
    }

    template <class E, class... T>
    Future<E, T...> &Future<E, T...>::operator=(Future &&other)
    {
        this->StateInstance::operator=(std::forward<StateInstance>(other));
        return *this;
    }

    template <class E, class... T>
    Future<E, T...> &Future<E, T...>::wait()
    {
        this->instance().wait();
        return *this;
    }

    template <class E, class... T>
    bool Future<E, T...>::resolved() const
    {
        return this->instance().resolved();
    }

    template <class E, class... T>
    bool Future<E, T...>::hasError()
    {
        wait();
        return this->instance().resolved2Error();
    }

    template <class E, class... T>
    bool Future<E, T...>::hasValue()
    {
        wait();
        return this->instance().resolved2Value();
    }

    template <class E, class... T>
    const std::tuple<T...> &Future<E, T...>::value()
    {
        wait();
        return this->instance().value();
    }

    template <class E, class... T>
    template <std::size_t idx>
    const typename std::tuple_element<idx, std::tuple<T...> >::type &Future<E, T...>::value()
    {
        wait();
        return std::get<idx>(this->instance().value());
    }

    template <class E, class... T>
    std::tuple<T...> &&Future<E, T...>::detachValue()
    {
        wait();
        return this->instance().value();
    }

    template <class E, class... T>
    template <std::size_t idx>
    typename std::tuple_element<idx, std::tuple<T...> >::type &&Future<E, T...>::detachValue()
    {
        wait();
        return std::get<idx>(std::forward<typename details::FutureState<E, T...>::Value>(this->instance().value()));
    }

    template <class E, class... T>
    const E &Future<E, T...>::error()
    {
        wait();
        return this->instance().error();
    }

    template <class E, class... T>
    E &&Future<E, T...>::detachError()
    {
        wait();
        return std::move(this->instance().error());
    }

    template <class E, class... T>
    template <class F>
    void Future<E, T...>::then(F &&f)
    {
        struct Then
            : details::FutureThenBase<E, T...>
        {
            Then(F &&call, const Future<E, T...> &srcFuture)
                : _call(std::forward<F>(call))
                , _srcFuture(srcFuture)
            {
            }

            void call(E *err, T *... vals) override
            {
                _call(err, vals...);
            }

            void destroy() override
            {
                this->~Then();
                mm::free<sizeof(Then)>(this);
            }

            std::decay_t<F>                 _call;
            Future<E, T...>                 _srcFuture;
        } *then = new(mm::alloc<sizeof(Then)>()) Then(std::forward<F>(f), *this);

        this->instance().pushThen(then);
    }

}}

#include "promise.hpp"

namespace dci { namespace async
{
    template <class E, class... T>
    template <class Et, class... Tt, class F>
    Future<Et, Tt...> Future<E, T...>::thenTransform(F &&f)
    {
        struct Then
            : details::FutureThenBase<E, T...>
        {
            Then(F &&call, const Future<E, T...> &srcFuture)
                : _call(std::forward<F>(call))
                , _srcFuture(srcFuture)
            {
            }

            void call(E *err, T *... vals) override
            {
                _call(err, vals..., _dstPromise);
            }

            void destroy() override
            {
                this->~Then();
                mm::free<sizeof(Then)>(this);
            }

            std::decay_t<F>                 _call;
            Future<E, T...>                 _srcFuture;
            dci::async::Promise<Et, Tt...>  _dstPromise;
        } *then = new(mm::alloc<sizeof(Then)>()) Then(std::forward<F>(f), *this);

        auto res = then->_dstPromise.future();
        this->instance().pushThen(then);
        return res;
    }

}}

