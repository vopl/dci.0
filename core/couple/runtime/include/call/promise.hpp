#pragma once

#include "error.hpp"

#include <dci/async/future.hpp>
#include <boost/variant.hpp>

#include <cassert>

namespace dci { namespace couple { namespace runtime { class call
{
    template <class R>
    class Promise
        : public dci::async::Promise<boost::variant<Error, R>>
    {
    public:
        Promise();
        Promise(Promise &&);
        ~Promise();

        Promise &operator=(Promise &&r);

    };

}}}}
