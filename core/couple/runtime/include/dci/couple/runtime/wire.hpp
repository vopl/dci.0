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
        typename Signal<R(Args...)>::Result operator()(Args &&... args);
    };


    template <class... Args>
    class Wire<nowaitvoid(Args...)>
        : public Signal<nowaitvoid(Args...)>
    {
        Wire(const Wire &) = delete;
        void operator=(const Wire &) = delete;

    public:
        Wire() = default;
        typename Signal<nowaitvoid(Args...)>::Result operator()(Args &&... args);
    };

    template <class R, class... Args>
    typename Signal<R(Args...)>::Result Wire<R(Args...)>::operator()(Args &&... args)
    {
        if(Signal<R(Args...)>::_call)
        {
            return Signal<R(Args...)>::_call(std::forward<Args>(args)...);
        }

        return typename Signal<R(Args...)>::Result(make_error_code(error::general::call_not_connected));
    }

    template <class... Args>
    typename Signal<nowaitvoid(Args...)>::Result Wire<nowaitvoid(Args...)>::operator()(Args &&... args)
    {
        if(Signal<nowaitvoid(Args...)>::_call)
        {
            Signal<nowaitvoid(Args...)>::_call(std::forward<Args>(args)...);
        }
    }

}}}
