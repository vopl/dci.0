#pragma once

#include <dci/couple/runtime.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/functor_row.hpp>

#include "streamer.hpp"
#include <cxxabi.h>

namespace impl { namespace fsm { namespace endpoint
{
    namespace msm = boost::msm;
    namespace mpl = boost::mpl;
    using namespace msm::front;

    using namespace dci::couple::runtime;
    using namespace streamer;

    template <class T>
    std::string demangle()
    {
        const char* name = typeid(T).name();
        int status = -1;
        std::unique_ptr<char, void(*)(void*)> res {
            abi::__cxa_demangle(name, NULL, NULL, &status),
            std::free
        };
        return (status==0) ? res.get() : name ;
    }

    template <typename State>
    struct BaseState : public state<>
    {
        template <class Event, class FSM> void on_entry(const Event &, FSM &)
        {
            std::cout << "on_entry: " << demangle<State>()  << std::endl;
        }
        template <class Event, class FSM> void on_exit(const Event &, FSM &)
        {
            std::cout << "on_exit: " << demangle<State>() << std::endl;
        }
    };

    template <typename MachineDef>
    struct BaseMachineDef : public state_machine_def<MachineDef>
    {
        using active_state_switch_policy = msm::active_state_switch_after_exit;

        template <class Fsm,class Event>
        void no_transition(Event const&, Fsm& ,int state)
        {
            LOGT("no_transition in "<<demangle<MachineDef>()<<": "<<demangle<Event>()<<", "<<state);
        }

        template <class Event, class FSM> void on_entry(const Event &, FSM &)
        {
            std::cout << "on_entry: " << demangle<MachineDef>()  << std::endl;
        }
        template <class Event, class FSM> void on_exit(const Event &, FSM &)
        {
            std::cout << "on_exit: " << demangle<MachineDef>() << std::endl;
        }

        using no_exception_thrown = int;
        using no_message_queue = int;
    };

    template <typename MachineDef, class... Args>
    using Machine = boost::msm::back::state_machine<MachineDef, Args...>;

}}}
