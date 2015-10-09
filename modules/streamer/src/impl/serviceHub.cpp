#include "serviceHub.hpp"

namespace impl
{
    template <class Derived>
    ServiceHub<Derived>::ServiceHub()
    {
    }

    template <class Derived>
    ServiceHub<Derived>::~ServiceHub()
    {
    }

    template <class Derived>
    void ServiceHub<Derived>::start()
    {

    }

    template <class Derived>
    void ServiceHub<Derived>::stop()
    {

    }

    template <class Derived>
    Future< streamer::ServiceHub::ServiceId> ServiceHub<Derived>::injectService(streamer::ServiceHub::Remote &&arg_0)
    {
        return _localLinks.add()->inject(std::move(arg_0));
    }

    template <class Derived>
    Future< streamer::ServiceHub::Local, streamer::ServiceHub::ServiceId> ServiceHub<Derived>::inviteService(Iid &&arg_0)
    {
        return _localLinks.add()->invite(std::move(arg_0));
    }

    template <class Derived>
    Future< streamer::ServiceHub::Remote> ServiceHub<Derived>::ejectService(streamer::ServiceHub::ServiceId &&arg_0)
    {
        return _localLinks.del(std::move(arg_0));
    }

}

#include "../handlers/serviceHub.hpp"
template class impl::ServiceHub<handlers::ServiceHub>;
