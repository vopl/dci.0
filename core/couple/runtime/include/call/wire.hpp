#pragma once

#include "signal.hpp"
#include "future.hpp"
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

        Future<R> operator()(Args... args);
    };


    template <class R, class... Args>
    Wire<R(Args...)>::Wire()
    {
        assert(0);
    }

    template <class R, class... Args>
    Wire<R(Args...)>::~Wire()
    {
        assert(0);
    }

    template <class R, class... Args>
    Future<R> Wire<R(Args...)>::operator()(Args... args)
    {
        assert(0);
    }

}}}}
