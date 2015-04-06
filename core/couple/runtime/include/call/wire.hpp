#pragma once

#include <dci/async/future.hpp>

#include "signal.hpp"
#include "error.hpp"
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

        typename Signal<R(Args...)>::Future operator()(ValuePorter<Args> &&... args);
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
    typename Signal<R(Args...)>::Future Wire<R(Args...)>::operator()(call::ValuePorter<Args> &&... args)
    {
        if(Signal<R(Args...)>::_call)
        {
            return Signal<R(Args...)>::_call(std::forward<call::ValuePorter<Args>>(args)...);
        }

        return typename Signal<R(Args...)>::Future(make_error_code(error::general::call_not_connected));
    }

}}}}
