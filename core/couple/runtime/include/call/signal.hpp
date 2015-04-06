#pragma once

#include <dci/async/future.hpp>

#include "error.hpp"
#include "valuePorter.hpp"
#include <functional>
#include <type_traits>

namespace dci { namespace couple { namespace runtime { namespace call
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<int> struct BindPlaceholder {};

    template <class F>
    class Signal;

    template <class R, class...Args>
    class Signal<R(Args...)>
    {

    public:
        Signal();
        ~Signal();

        template <class F, class... LocalArgs>
        bool connect(F &&f, LocalArgs &&... localArgs);

        bool disconnect();

    protected:
        using Future = std::conditional_t<
            std::is_same<void, R>::value,
            dci::async::Future<Error>,
            dci::async::Future<Error, ValuePorter<R>>>;

        using Promise = typename Future::Promise;

        using Call = std::function<Future(ValuePorter<Args> &&...)>;
        Call _call;
    };


    template <class R, class...Args>
    Signal<R(Args...)>::Signal()
    {

    }

    template <class R, class...Args>
    Signal<R(Args...)>::~Signal()
    {

    }

    namespace details
    {
        template <class F, class... LocalArgs, int... phi>
        decltype(std::bind(std::declval<F>(), std::declval<LocalArgs>()..., BindPlaceholder<phi>{}...))
            mkCall(std::integer_sequence<int, phi...>, F &&f, LocalArgs &&... localArgs)
        {
            return std::bind(std::forward<F>(f), std::forward<LocalArgs>(localArgs)..., BindPlaceholder<phi>{}...);
        }
    }


    template <class R, class...Args>
    template <class F, class... LocalArgs>
    bool Signal<R(Args...)>::connect(F &&f, LocalArgs &&... localArgs)
    {
        if(_call)
        {
            assert(!"signal already connected");
            return false;
        }

        _call = details::mkCall(
                        std::make_integer_sequence<int, sizeof... (Args)>{},
                        std::forward<F>(f),
                        std::forward<LocalArgs>(localArgs)...);
        return true;
    }

    template <class R, class...Args>
    bool Signal<R(Args...)>::disconnect()
    {
        if(!_call)
        {
            return false;
        }

        _call = Call{};
        return true;
    }

}}}}

namespace std
{
    template<int N>
    struct is_placeholder<dci::couple::runtime::call::BindPlaceholder<N>>
        : integral_constant<int, N+1>
    {};
}

