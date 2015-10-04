#pragma once

#include "events.hpp"
#include "attached/reading.hpp"
#include "attached/readSubscription.hpp"
#include "attached/writing.hpp"

namespace impl { namespace fsm { namespace endpoint
{
    using namespace attached;

    struct AttachedDef : BaseMachineDef<AttachedDef>
    {
    public://subregions joiner
        struct AllJoined {};


        struct Join : BaseState<Join>
        {
            template <class Event, class FSM>
            void on_entry(const Event &evt, FSM &fsm)
            {
                BaseState<Join>::on_entry(evt, fsm);
                fsm._joinCounter++;
                if(mpl::size<initial_state>::value == fsm._joinCounter)
                {
                    fsm.process_event(AllJoined());
                }
            }
        };

    public: //exit pseudo state
        struct Exit : public exit_pseudo_state<none> {};


    public:
        std::size_t _joinCounter;
        Channel _channel;


    public:
        template <class FSM>
        void on_entry(const inEvents::common::Attach &evt, FSM &fsm)
        {
            BaseMachineDef<AttachedDef>::on_entry(evt, fsm);
            _joinCounter = 0;
            _channel = std::move(evt._channel);
            fsm.resolveAttachPromise();
        }

        template <class Event, class FSM>
        void on_exit(const Event &evt, FSM &fsm)
        {
            BaseMachineDef<AttachedDef>::on_exit(evt, fsm);
            fsm.resolveDetachPromise(std::move(_channel));
        }

    public:
        using initial_state = mpl::vector<
            Reading,
            ReadSubscription,
            Writing
        >;

        using transition_table = mpl::vector<
            Row<    Join,                                               AllJoined,                  Exit,   none,   none>,

            Row<    Reading::exit_pt<Reading::Done>,                    none,                       Join,   none,   none>,
            Row<    ReadSubscription::exit_pt<ReadSubscription::Done>,  none,                       Join,   none,   none>,
            Row<    Writing,                                            inEvents::common::Detach,   Join,   none,   none>
        >;
    };

    using Attached = Machine<AttachedDef>;

}}}
