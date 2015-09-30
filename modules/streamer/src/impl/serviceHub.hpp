#pragma once

#include <dci/couple/runtime.hpp>
#include "streamer.hpp"

namespace impl
{
    using namespace dci::couple::runtime;
    using namespace streamer;

    template <class Derived>
    class ServiceHub
    {

    public:
        Future< streamer::ServiceHub::ServiceId> injectService(Interface &&arg_0);
        Future< Interface, streamer::ServiceHub::ServiceId> inviteService(Iid &&arg_0);
        Future< Interface> ejectService(streamer::ServiceHub::ServiceId &&arg_0);
    };
}
