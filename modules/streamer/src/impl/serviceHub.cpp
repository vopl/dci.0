#include "serviceHub.hpp"

namespace impl
{
    template <class Derived>
    Future< streamer::ServiceHub::ServiceId> ServiceHub<Derived>::injectService(Interface &&arg_0)
    {
        (void)arg_0;
        assert(0);
    }

    template <class Derived>
    Future< Interface, streamer::ServiceHub::ServiceId> ServiceHub<Derived>::inviteService(Iid &&arg_0)
    {
        (void)arg_0;
        assert(0);
    }

    template <class Derived>
    Future< Interface> ServiceHub<Derived>::ejectService(streamer::ServiceHub::ServiceId &&arg_0)
    {
        (void)arg_0;
        assert(0);
    }

}

#include "../handlers/serviceHub.hpp"
template class impl::ServiceHub<handlers::ServiceHub>;
