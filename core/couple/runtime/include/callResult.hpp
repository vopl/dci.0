#pragma once

#include <dci/async/future.hpp>

#include <cassert>

namespace dci { namespace couple { namespace runtime
{
    template <class R>
    class CallResult
    {
    public:
        CallResult();
        CallResult(const R &);
        CallResult(R &&);
        CallResult(const dci::async::Future<R> &);
        CallResult(dci::async::Future<R> &&);
        CallResult(const CallResult &);
        CallResult(CallResult &&);
        ~CallResult();

        CallResult &operator=(const R &r);
        CallResult &operator=(R &&r);

        CallResult &operator=(dci::async::Future<R> &&r);
        CallResult &operator=(const dci::async::Future<R> &r);

        CallResult &operator=(CallResult &&r);
        CallResult &operator=(const CallResult &r);



        bool hasError() const;
        const R &value() const;
    };


    template <class R>
    CallResult<R>::CallResult()
    {
        assert(0);
    }

    template <class R>
    CallResult<R>::CallResult(const R &)
    {
        assert(0);
    }

    template <class R>
    CallResult<R>::CallResult(R &&)
    {
        assert(0);
    }

    template <class R>
    CallResult<R>::CallResult(const dci::async::Future<R> &)
    {
        assert(0);
    }

    template <class R>
    CallResult<R>::CallResult(dci::async::Future<R> &&)
    {
        assert(0);
    }

    template <class R>
    CallResult<R>::CallResult(const CallResult &)
    {
        assert(0);
    }

    template <class R>
    CallResult<R>::CallResult(CallResult &&)
    {
        assert(0);
    }

    template <class R>
    CallResult<R>::~CallResult()
    {
        assert(0);
    }

    template <class R>
    CallResult<R> &CallResult<R>::operator=(const R &r)
    {
        assert(0);
    }

    template <class R>
    CallResult<R> &CallResult<R>::operator=(R &&r)
    {
        assert(0);
    }

    template <class R>
    CallResult<R> &CallResult<R>::operator=(dci::async::Future<R> &&r)
    {
        assert(0);
    }

    template <class R>
    CallResult<R> &CallResult<R>::operator=(const dci::async::Future<R> &r)
    {
        assert(0);
    }

    template <class R>
    CallResult<R> &CallResult<R>::operator=(CallResult &&r)
    {
        assert(0);
    }

    template <class R>
    CallResult<R> &CallResult<R>::operator=(const CallResult &r)
    {
        assert(0);
    }

    template <class R>
    bool CallResult<R>::hasError() const
    {
        assert(0);
    }

    template <class R>
    const R &CallResult<R>::value() const
    {
        assert(0);
    }

}}}
