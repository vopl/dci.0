#pragma once

#include <dci/logger.hpp>
#include "streamer.hpp"

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

namespace impl { namespace fsm
{
    namespace msm = boost::msm;
    namespace mpl = boost::mpl;
    using namespace msm::front;

    using namespace dci::couple::runtime;
    using namespace streamer;

    struct Reader : msm::front::state_machine_def<Reader>
    {
    public://events
        struct data
        {
        };


    public://states
        struct WaitHeader : msm::front::state<>
        {
        };

        struct WaitBody : msm::front::state<>
        {
        };
        friend struct Attached;


    public:
        using no_exception_thrown = int;
        using no_message_queue = int;

        using initial_state = WaitHeader;

        using transition_table = mpl::vector<
            Row<    WaitHeader,   data, WaitBody,   none,   none    >,
            Row<    WaitBody,   data, WaitHeader,   none,   none    >
        >;


    public:
        template <class Fsm,class Event>
        void no_transition(Event const& e, Fsm& ,int state)
        {
            LOGT("no_transition: "<<typeid(e).name()<<", "<<state);
        }
    };
}}
