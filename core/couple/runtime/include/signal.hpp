#pragma once

#include <dci/async/future.hpp>
#include "callResult.hpp"
#include <functional>
#include <type_traits>

namespace dci { namespace couple { namespace runtime
{
    template <class F>
    class Signal;

    template <class R, class...Args>
    class Signal<R(Args...)>
    {

    public:
        template <class F>
        std::enable_if_t<
                std::is_same<R,decltype(std::declval<F>()(std::declval<Args>...))>::value ||
                std::is_same<dci::async::Future<R>,decltype(std::declval<F>()(std::declval<Args>...))>::value
                , bool>
        connect(F &&f)
        {
            if(_function)
            {
                return false;
            }

            _function = [f=std::move(f)](Args... args) -> CallResult<R>
            {
                CallResult<R> r;
                r = f(std::forward<Args>(args)...);
                return r;
            };

            return true;
        }

        template <class F>
        std::enable_if_t<
                std::is_same<CallResult<R>,decltype(std::declval<F>()(std::declval<Args>...))>::value
                , bool>
        connect(F &&f)
        {
            if(_function)
            {
                return false;
            }

            _function = f;

            return true;
        }

        bool disconnect()
        {
            if(_function)
            {
                _function = std::function<CallResult<R>(Args...)>{};
                return true;
            }

            return false;
        }

    protected:
        CallResult<R> fire(Args...);
        std::function<CallResult<R>(Args...)> _function;
    };




    template <class R, class...Args>

    CallResult<R> Signal<R(Args...)>::fire(Args...)
    {
        if(_function)
        {
            return _function();
        }

        CallResult<R> r;
        assert(0);
//        r.setError(not connected);
        return r;
    }

}}}
