#pragma once

#include "../events.hpp"

namespace impl { namespace fsm  { namespace endpoint  { namespace attached
{
    struct ReadingDef : BaseMachineDef<ReadingDef>
    {
    public:
        struct Continuous : BaseState<Continuous> {};
        struct Tail : BaseState<Tail>
        {
            template <class Event, class FSM> void on_entry(const Event &evt, FSM &fsm)
            {
                BaseState<Tail>::on_entry(evt, fsm);
            }

        };
        struct Done : public exit_pseudo_state<none> {};

        struct testAction
        {
            template <class Event, class FSM, class Src, class Tgt>
            void operator()(const Event &, FSM &fsm, Src &, Tgt &)
            {
//                LOGT("FlowComplete process");
//                fsm.process_event(inEvents::read::FlowComplete());
//                LOGT("FlowComplete processed");
            }
        };

        struct IsFlowComplete
        {
            template <class Event, class FSM, class Src, class Tgt>
            bool operator()(const Event &, FSM &, Src &, Tgt &)
            {
                return true;
            }
        };


    public:
        using initial_state = Continuous;

        using transition_table = mpl::vector<
            Row<Continuous, inEvents::common::Detach,       Tail, none, none>,
            Row<Continuous, inEvents::read::Flow,           none, none, none>,

            Row<Tail,       none,                           Done, none, IsFlowComplete>,
            Row<Tail,       inEvents::read::Flow,           none, none, none>
        >;



    };
    using Reading = Machine<ReadingDef>;

}}}}
