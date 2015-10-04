#pragma once

#include "../events.hpp"

namespace impl { namespace fsm  { namespace endpoint  { namespace attached
{
    struct ReadingDef : BaseMachineDef<ReadingDef>
    {
        struct S1 : BaseState<S1> {};
        struct Tail : public exit_pseudo_state<none> {};

        using initial_state = S1;

        using transition_table = mpl::vector<
            Row<S1, inEvents::common::Detach, Tail, none, none>
        >;



    };
    using Reading = boost::msm::back::state_machine<ReadingDef>;

}}}}
