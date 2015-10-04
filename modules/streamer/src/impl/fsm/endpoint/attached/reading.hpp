#pragma once

#include "../events.hpp"

namespace impl { namespace fsm  { namespace endpoint  { namespace attached
{
    struct ReadingDef : BaseMachineDef<ReadingDef>
    {
    public:
        struct Continuous : BaseState<Continuous> {};
        struct Tail : BaseState<Continuous> {};
        struct Done : public exit_pseudo_state<none> {};

    public:
        using initial_state = Continuous;

        using transition_table = mpl::vector<
            Row<Continuous, inEvents::common::Detach,       Tail, none, none>,
            Row<Tail,       inEvents::read::FlowComplete,   Done, none, none>
        >;



    };
    using Reading = boost::msm::back::state_machine<ReadingDef>;

}}}}
