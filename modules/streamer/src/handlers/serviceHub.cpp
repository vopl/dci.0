#include "serviceHub.hpp"
#include <dci/couple/runtime.hpp>

namespace handlers
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    ServiceHub::ServiceHub()
    {
        _fsm.start();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    ServiceHub::~ServiceHub()
    {
        detachChannel().wait();
        _fsm.stop();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< > ServiceHub::attachChannel(Channel &&arg_0)
    {
        LOGT("attach");
        if(!arg_0._input || !arg_0._output)
        {
            return make_error_code(streamer::error::badChannelValue);
        }

        auto res = _fsm.getAttachFuture();
        _fsm.process_event(::impl::fsm::inEvents::common::Attach{std::move(arg_0)});
        return res;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< Channel> ServiceHub::detachChannel()
    {
        LOGT("detach");
        auto res = _fsm.getDetachFuture();
        _fsm.process_event(::impl::fsm::inEvents::common::Detach());
        return res;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< streamer::ServiceHub::ServiceId> ServiceHub::injectService(Interface &&arg_0)
    {
        (void)arg_0;
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< Interface, streamer::ServiceHub::ServiceId> ServiceHub::inviteService(Iid &&arg_0)
    {
        (void)arg_0;
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< Interface> ServiceHub::ejectService(streamer::ServiceHub::ServiceId &&arg_0)
    {
        (void)arg_0;
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void ServiceHubHandlerFactory::createService(void *outFuture)
    {
        auto *res = static_cast<dci::async::Future<std::error_code, ::streamer::ServiceHub> *>(outFuture);
        *res = ::streamer::ServiceHub(* new ServiceHub);
    }

}
