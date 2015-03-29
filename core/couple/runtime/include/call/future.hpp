#pragma once

#include "error.hpp"

#include <dci/async/future.hpp>
#include <boost/variant.hpp>

#include <cassert>

namespace dci { namespace couple { namespace runtime { namespace call
{

    template <class R>
    class Future
        : public dci::async::Future<boost::variant<Error, R>>
    {
    public:
        Future();
        Future(const Future &);
        Future(Future &&);
        ~Future();

        Future &operator=(Future &&r);
        Future &operator=(const Future &r);

        bool hasError() const;
        const Error &error();

        bool hasValue() const;
        const R &value();
        R &&detachValue();
    };

}}}}
