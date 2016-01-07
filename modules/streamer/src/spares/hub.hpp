#pragma once

#include <dci/couple/runtime.hpp>
#include <dci/async.hpp>

#include "hub/pool.hpp"
using namespace streamer;

namespace spares
{
    using namespace dci::couple::runtime;


    template <class Derived, class LocalLink, class RemoteLink>
    class Hub
    {
    public:
        Hub();
        ~Hub();

    public:
        Future< ServiceHub::ServiceId> injectService(ServiceHub::Remote &&arg_0);
        Future< ServiceHub::Local, ServiceHub::ServiceId> inviteService(const Iid &arg_0);
        Future< ServiceHub::Remote> ejectService(const ServiceHub::ServiceId &arg_0);

    public:
        template <class Message>
        void input(Message &&message);

    private:
        hub::Pool<LocalLink> _localLinks;
        hub::Pool<RemoteLink> _remoteLinks;
    };









    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Derived, class LocalLink, class RemoteLink>
    Hub<Derived, LocalLink, RemoteLink>::Hub()
    {

    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Derived, class LocalLink, class RemoteLink>
    Hub<Derived, LocalLink, RemoteLink>::~Hub()
    {

    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Derived, class LocalLink, class RemoteLink>
    Future< ServiceHub::ServiceId> Hub<Derived, LocalLink, RemoteLink>::injectService(ServiceHub::Remote &&arg_0)
    {
        (void)arg_0;
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Derived, class LocalLink, class RemoteLink>
    Future< ServiceHub::Local, ServiceHub::ServiceId> Hub<Derived, LocalLink, RemoteLink>::inviteService(const Iid &arg_0)
    {
        (void)arg_0;
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Derived, class LocalLink, class RemoteLink>
    Future< ServiceHub::Remote> Hub<Derived, LocalLink, RemoteLink>::ejectService(const ServiceHub::ServiceId &arg_0)
    {
        (void)arg_0;
        assert(0);
    }

}
