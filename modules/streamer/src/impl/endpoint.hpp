#pragma once

#include <dci/couple/runtime.hpp>
#include "streamer.hpp"

namespace impl
{
    using namespace dci::couple::runtime;
    using namespace streamer;

    template <class Derived>
    class Endpoint
    {

    public:
        Future< > attachChannel(Channel &&arg_0);
        Future< Channel> detachChannel();
    };
}
