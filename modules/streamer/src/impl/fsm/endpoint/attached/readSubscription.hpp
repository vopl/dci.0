#pragma once

#include "../events.hpp"

namespace impl { namespace fsm  { namespace endpoint  { namespace attached
{
    struct ReadSubscriptionDef : BaseMachineDef<ReadSubscriptionDef>
    {
    public:
        struct Stable : BaseState<Stable> {};
        struct WaitResult : BaseState<WaitResult> {};
        struct Fail : BaseState<Fail> {};
        struct WaitFinalResult : BaseState<WaitFinalResult> {};
        struct WaitResultAfterStop : BaseState<WaitResultAfterStop> {};
        struct Done : public exit_pseudo_state<none> {};

    public:
        using initial_state = Stable;

        using transition_table = mpl::vector<

            Row<Stable,             inEvents::common::Detach,               WaitFinalResult,        none, none>,
            Row<Stable,             inEvents::read::SubscribeNone,          WaitResult,             none, none>,
            Row<Stable,             inEvents::read::SubscribeStepByStep,    WaitResult,             none, none>,
            Row<Stable,             inEvents::read::SubscribeContinuous,    WaitResult,             none, none>,

            Row<WaitResult,         inEvents::common::Detach,               WaitResultAfterStop,    none, none>,
            Row<WaitResult,         inEvents::read::SubscriptionOk,         Stable,                 none, none>,
            Row<WaitResult,         inEvents::read::SubscriptionFail,       Fail,                   none, none>,

            Row<Fail,               inEvents::common::Detach,               Done,                   none, none>,
            Row<Fail,               inEvents::read::Reset,                  Stable,                 none, none>,

            Row<WaitFinalResult,    inEvents::read::SubscriptionOk,         Done,                   none, none>,
            Row<WaitFinalResult,    inEvents::read::SubscriptionFail,       Done,                   none, none>,

            Row<WaitFinalResult,    inEvents::read::SubscriptionOk,         WaitFinalResult,        none, none>,
            Row<WaitFinalResult,    inEvents::read::SubscriptionFail,       Done,                   none, none>

        >;


    };

    using ReadSubscription = boost::msm::back::state_machine<ReadSubscriptionDef>;

}}}}
