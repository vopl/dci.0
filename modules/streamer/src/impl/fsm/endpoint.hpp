#pragma once

#include <dci/logger.hpp>
#include "streamer.hpp"

#include "endpoint/events.hpp"
#include "endpoint/detached.hpp"
#include "endpoint/attached.hpp"

namespace impl { namespace fsm
{
    using namespace endpoint;

    struct EndpointDef : BaseMachineDef<EndpointDef>
    {
    public:
        void resolveAttachPromise()
        {
            _attachPromise.resolveValue();
            _attachPromise = Promise<>();
        }

        void resolveDetachPromise(Channel &&channel)
        {
            _detachPromise.resolveValue(std::move(channel));
            _detachPromise = Promise<Channel>();
        }

        Future<> getAttachFuture()
        {
            return _attachPromise.future();
        }

        Future<Channel> getDetachFuture()
        {
            return _detachPromise.future();
        }

    private:
        Promise<>           _attachPromise;
        Promise<Channel>    _detachPromise;

    public:
        using initial_state = mpl::vector<Detached>;

        using transition_table = mpl::vector<
            Row<    Detached,                           inEvents::common::Attach,   Attached,   none,   none    >,
            Row<    Attached::exit_pt<Attached::Exit>,  none,                       Detached,   none,   none    >
        >;

    public:
        template <class Fsm>
        void no_transition(const inEvents::common::Attach &, Fsm &, int state)
        {
            assert(1 == state);
            _attachPromise.resolveError(streamer::error::alreadyAttached);
        }

        template <class Fsm>
        void no_transition(const inEvents::common::Detach &, Fsm &, int state)
        {
            assert(0 == state);
            _detachPromise.resolveError(streamer::error::alreadyDetached);
        }

        template <class Event, class Fsm>
        void no_transition(const Event &evt, Fsm &fsm, int state)
        {
            BaseMachineDef<EndpointDef>::no_transition(evt, fsm, state);
            assert(!"never here");
        }
    };

    using Endpoint = Machine<EndpointDef>;

}}
