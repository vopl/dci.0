#pragma once

#include "../events.hpp"

namespace impl { namespace fsm  { namespace endpoint  { namespace attached
{
    struct WritingDef : BaseMachineDef<WritingDef>
    {

        struct S1 : BaseState<S1> {};
        struct S2 : BaseState<S2> {};
        using initial_state = S1;

        using transition_table = mpl::vector<
            Row<S1, none, S2, none, none>
        >;
    };

    using Writing = boost::msm::back::state_machine<WritingDef>;

}}}}
