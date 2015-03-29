#pragma once

#include <dci/async/future.hpp>
#include <boost/variant.hpp>
#include <boost/variant/get.hpp>

#include <system_error>

#include <cassert>

namespace dci { namespace couple { namespace runtime { namespace call
{

    template <class R>
    class Future
        : public dci::async::Future<boost::variant<std::error_code, R>>
    {
        using Base = dci::async::Future<boost::variant<std::error_code, R>>;

    public:
        Future(const std::error_code &err);
        Future(std::error_code &&err);

        Future(const R &r);
        Future(R &&r);

        using Base::Base;
        using Base::operator=;

        bool hasError();
        const std::error_code &error();

        bool hasValue();
        const R &value();
        R &&detachValue();
    };



    template <class R>
    class Promise
        : public dci::async::Promise<boost::variant<std::error_code, R>>
    {
        using Base = dci::async::Promise<boost::variant<std::error_code, R>>;

    public:
        using Base::Base;
        using Base::operator=;

    };




    template <class R>
    Future<R>::Future(const std::error_code &err)
        : Base{boost::variant<std::error_code, R>(err)}
    {
    }

    template <class R>
    Future<R>::Future(std::error_code &&err)
        : Base{boost::variant<std::error_code, R>(std::forward<std::error_code>(err))}
    {
    }

    template <class R>
    Future<R>::Future(const R &r)
        : Base{boost::variant<std::error_code, R>(r)}
    {
    }

    template <class R>
    Future<R>::Future(R &&r)
        : Base{boost::variant<std::error_code, R>(std::forward<R>(r))}
    {
    }

    template <class R>
    bool Future<R>::hasError()
    {
        wait();
        return boost::get<std::error_code>(&Base::template value<0>()) ? true : false;
    }

    template <class R>
    const std::error_code &Future<R>::error()
    {
        wait();
        return boost::get<std::error_code>(Base::template value<0>());
    }

    template <class R>
    bool Future<R>::hasValue()
    {
        wait();
        return boost::get<R>(&Base::template value<0>());
    }

    template <class R>
    const R &Future<R>::value()
    {
        wait();
        return boost::get<R>(Base::template value<0>());
    }

    template <class R>
    R &&Future<R>::detachValue()
    {
        wait();
        return boost::get<R>(Base::template value<0>());
    }

}}}}
