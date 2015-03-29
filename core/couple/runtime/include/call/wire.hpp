#pragma once

#include "signal.hpp"
#include "future.hpp"
#include "../error.hpp"
#include <cassert>

namespace dci { namespace couple { namespace runtime { namespace call
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
        Wire();
        ~Wire();

        Future<R> operator()(Args &&... args);
    };


    template <class R, class... Args>
    Wire<R(Args...)>::Wire()
    {
    }

    template <class R, class... Args>
    Wire<R(Args...)>::~Wire()
    {
    }

    template <class R, class... Args>
    Future<R> Wire<R(Args...)>::operator()(Args &&... args)
    {
        if(Signal<R(Args...)>::_call)
        {
            return Signal<R(Args...)>::_call(std::forward<Args>(args)...);
        }

        return Future<R>{make_error_code(error::general::call_not_connected)};
    }

}}}}
