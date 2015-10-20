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
        _localLinks.clean([](auto){});
        _remoteLinks.clean([](auto){});
    }

    template <class Derived>
    void ServiceHub<Derived>::start()
    {
        assert(_localLinks.isEmpty());
        assert(_remoteLinks.isEmpty());

        _localLinks.setIdPrefix(0);
        _remoteLinks.setIdPrefix(1);
    }

    template <class Derived>
    void ServiceHub<Derived>::stop()
    {
        _localLinks.clean([](std::unique_ptr<serviceHub::LocalLink> &&l)
        {
            assert(l);
            l->stop();
        });
        _remoteLinks.clean([](std::unique_ptr<serviceHub::RemoteLink> &&l)
        {
            assert(l);
            l->stop();
        });
    }

    template <class Derived>
    Future< streamer::ServiceHub::ServiceId> ServiceHub<Derived>::injectService(streamer::ServiceHub::Remote &&arg_0)
    {
        serviceHub::LocalLink *link = _localLinks.add();
        if(!link)
        {
            return make_error_code(streamer::error::tooManyServices);
        }
        return link->inject(std::move(arg_0));
    }

    template <class Derived>
    Future< streamer::ServiceHub::Local, streamer::ServiceHub::ServiceId> ServiceHub<Derived>::inviteService(Iid &&arg_0)
    {
        serviceHub::LocalLink *link = _localLinks.add();
        if(!link)
        {
            return make_error_code(streamer::error::tooManyServices);
        }
        return link->invite(std::move(arg_0));
    }

    template <class Derived>
    Future< streamer::ServiceHub::Remote> ServiceHub<Derived>::ejectService(streamer::ServiceHub::ServiceId &&arg_0)
    {
        if(_localLinks.isPrefixSame(arg_0))
        {
            std::unique_ptr<serviceHub::LocalLink> link = _localLinks.del(std::move(arg_0));
            if(!link)
            {
                return make_error_code(streamer::error::badServiceId);
            }
            return link->eject();
        }

        std::unique_ptr<serviceHub::RemoteLink> link = _remoteLinks.del(std::move(arg_0));
        if(!link)
        {
            return make_error_code(streamer::error::badServiceId);
        }
        return link->eject();
    }

}

#include "../handlers/serviceHub.hpp"
template class impl::ServiceHub<handlers::ServiceHub>;
