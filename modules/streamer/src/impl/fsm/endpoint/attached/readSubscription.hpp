#pragma once

#include "../events.hpp"

namespace impl { namespace fsm  { namespace endpoint  { namespace attached
{
    struct ReadSubscriptionDef : BaseMachineDef<ReadSubscriptionDef>
    {
        struct S1 : BaseState<S1> {};
        struct Done : public exit_pseudo_state<none> {};

        using initial_state = S1;

        using transition_table = mpl::vector<
            Row<S1, inEvents::common::Detach, Done, none, none>
        >;


    };

    using ReadSubscription = boost::msm::back::state_machine<ReadSubscriptionDef>;

}}}}
