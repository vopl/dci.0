#pragma once

#include <dci/async/future.hpp>
#include "future.hpp"
#include <functional>
#include <type_traits>

namespace dci { namespace couple { namespace runtime { namespace call
{
    template <class F>
    class Signal;

    template <class R, class...Args>
    class Signal<R(Args...)>
    {

    public:
        template <class F>
        bool connect(F &&f);

        bool disconnect();

    protected:
        Future<R> fire(Args...);
    };


    template <class R, class...Args>
    template <class F>
    bool Signal<R(Args...)>::connect(F &&f)
    {
        assert(0);
    }

    template <class R, class...Args>
    bool Signal<R(Args...)>::disconnect()
    {
        assert(0);
    }

    template <class R, class...Args>
    Future<R> Signal<R(Args...)>::fire(Args...)
    {
        assert(0);
    }

}}}}
