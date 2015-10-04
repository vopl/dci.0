#pragma once

#include "../events.hpp"

namespace impl { namespace fsm  { namespace endpoint  { namespace attached
{
    struct WritingDef : BaseMachineDef<WritingDef>
    {
    public:
        struct OkDef : BaseMachineDef<OkDef>
        {
        public:
            struct None : BaseState<None> {};
            struct StepByStep : BaseState<StepByStep> {};
            struct Continuous : BaseState<Continuous> {};

        public:
            using initial_state = None;

            using transition_table = mpl::vector<
                Row<None,       inEvents::write::SubscribeStepByStep,   StepByStep,     none, none>,
                Row<None,       inEvents::write::SubscribeContinuous,   Continuous,     none, none>,
                Row<None,       inEvents::write::Message,               none,           none, none>,
                Row<None,       inEvents::write::MessageOk,             none,           none, none>,

                Row<StepByStep, inEvents::write::SubscribeNone,         None,           none, none>,
                Row<StepByStep, inEvents::write::SubscribeContinuous,   Continuous,     none, none>,
                Row<StepByStep, inEvents::write::Message,               none,           none, none>,
                Row<StepByStep, inEvents::write::MessageOk,             none,           none, none>,

                Row<Continuous, inEvents::write::SubscribeStepByStep,   StepByStep,     none, none>,
                Row<Continuous, inEvents::write::SubscribeNone,         None,           none, none>,
                Row<Continuous, inEvents::write::Message,               none,           none, none>,
                Row<Continuous, inEvents::write::MessageOk,             none,           none, none>
            >;
        };

        using Ok = boost::msm::back::state_machine<OkDef>;

        struct Fail : BaseState<Fail> {};

    public:
        using initial_state = Ok;

        using transition_table = mpl::vector<
            Row<Ok,     inEvents::write::MessageFail,   Fail,   none, none>,
            Row<Fail,   inEvents::write::Reset,         Ok,     none, none>
        >;
    };

    using Writing = boost::msm::back::state_machine<WritingDef>;

}}}}
