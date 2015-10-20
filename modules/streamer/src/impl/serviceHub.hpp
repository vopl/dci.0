#pragma once

#include <dci/couple/runtime.hpp>
#include "streamer.hpp"

#include "links/pool.hpp"
#include "serviceHub/localLink.hpp"
#include "serviceHub/remoteLink.hpp"

namespace impl
{
    using namespace dci::couple::runtime;
    using namespace streamer;

    template <class Derived>
    class ServiceHub
    {
    protected:
        ServiceHub();
        ~ServiceHub();

    public:
        void start();
        void stop();

    public:
        Future< streamer::ServiceHub::ServiceId> injectService(streamer::ServiceHub::Remote &&arg_0);
        Future< streamer::ServiceHub::Local, streamer::ServiceHub::ServiceId> inviteService(Iid &&arg_0);
        Future< streamer::ServiceHub::Remote> ejectService(streamer::ServiceHub::ServiceId &&arg_0);

    private:
        links::Pool<serviceHub::LocalLink> _localLinks;
        links::Pool<serviceHub::RemoteLink> _remoteLinks;
    };
}
