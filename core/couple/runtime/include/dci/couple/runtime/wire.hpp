#pragma once

#include <dci/async/future.hpp>

#include "signal.hpp"
#include "error.hpp"
#include <cassert>

namespace dci { namespace couple { namespace runtime
{
    template <class F>
    class Wire;

    template <class R, class... Args>
    class Wire<R(Args...)>
        : public Signal<R(Args...)>
    {
        Wire(const Wire &) = delete;
        void operator=(const Wire &) = delete;

    public:
        Wire() = default;
        R operator()(Args &&... args);
    };


    template <class... Args>
    class Wire<void(Args...)>
        : public Signal<void(Args...)>
    {
        Wire(const Wire &) = delete;
        void operator=(const Wire &) = delete;

    public:
        Wire() = default;
        void operator()(Args &&... args);
    };

    template <class R, class... Args>
    R Wire<R(Args...)>::operator()(Args &&... args)
    {
        if(Signal<R(Args...)>::_call)
        {
            return Signal<R(Args...)>::_call(std::forward<Args>(args)...);
        }

        return R(make_error_code(err_general::call_not_connected));
    }

    template <class... Args>
    void Wire<void(Args...)>::operator()(Args &&... args)
    {
        if(Signal<void(Args...)>::_call)
        {
            Signal<void(Args...)>::_call(std::forward<Args>(args)...);
        }
    }

}}}
