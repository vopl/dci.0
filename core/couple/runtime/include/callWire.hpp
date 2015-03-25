#pragma once

#include "callResult.hpp"
#include "signal.hpp"
#include <cassert>

namespace dci { namespace couple { namespace runtime
{
    template <class F>
    class CallWire;

    template <class R, class... Args>
    class CallWire<R(Args...)>
        : public Signal<R(Args...)>
    {
        CallWire(const CallWire &) = delete;
        void operator=(const CallWire &) = delete;

    public:
        CallWire();
        ~CallWire();

        CallResult<R> operator()(Args... args);
    };



    template <class R, class... Args>
    CallWire<R(Args...)>::CallWire()
    {
    }

    template <class R, class... Args>
    CallWire<R(Args...)>::~CallWire()
    {
    }

    template <class R, class... Args>
    CallResult<R> CallWire<R(Args...)>::operator()(Args... args)
    {
        return Signal<R(Args...)>::fire(std::forward<Args>(args)...);
    }

}}}
