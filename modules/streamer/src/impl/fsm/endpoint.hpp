#pragma once

#include <dci/logger.hpp>
#include "streamer.hpp"

//TODO:  не так. Одна субмашина Аттачед, внутри нее ортогональные регионы подписки, чтения, записи
#include "endpoint/reader.hpp"
#include "endpoint/readerSubscription.hpp"
#include "endpoint/writer.hpp"

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

namespace impl { namespace fsm
{
    namespace msm = boost::msm;
    namespace mpl = boost::mpl;
    using namespace msm::front;

    using namespace dci::couple::runtime;
    using namespace streamer;

    struct Endpoint : msm::front::state_machine_def<Endpoint>
    {
    public:
        Future< > attachFuture()
        {
            return _attachPromise.future();
        }

        Future< Channel> detachFuture()
        {
            return _detachPromise.future();
        }

    public://events
        struct attach
        {
            mutable Channel _channel;
        };

        struct detach
        {
        };

    public://states
        struct Detached : public msm::front::state<>
        {
            template <class Event,class Fsm> void on_entry(const Event &,Fsm &fsm)
            {
                fsm._attachPromise = Promise< >();
            }

            template <class Fsm> void on_entry(const detach &,Fsm &fsm)
            {
                fsm._attachPromise = Promise< >();
                fsm._detachPromise.resolveValue(std::move(fsm._channel));
            }

            template <class Event,class Fsm> void on_exit(const Event &,Fsm &)
            {
            }
        };

        struct Attached : msm::front::state_machine_def<Attached>
        {
            template <class Event,class Fsm> void on_entry(const Event &,Fsm &fsm)
            {
                fsm._detachPromise = Promise< Channel>();
                fsm._attachPromise.resolveValue();
            }

            template <class Fsm> void on_entry(const attach &evt,Fsm &fsm)
            {
                fsm._detachPromise = Promise< Channel>();
                fsm._channel = std::move(evt._channel);
                fsm._attachPromise.resolveValue();
            }

            template <class Event,class Fsm> void on_exit(const Event &,Fsm &)
            {
            }

            using no_exception_thrown = int;
            using no_message_queue = int;

            using initial_state = Reader;

            using transition_table = mpl::vector<
                Row<    Reader,   none, Reader,   none,   none    >
            >;

        };
        friend struct Attached;


    public:
        using no_exception_thrown = int;
        using no_message_queue = int;

        using initial_state = Detached;

        using transition_table = mpl::vector<
            Row<    Detached,   attach, Attached,   none,   none    >,
            Row<    Attached,   detach, Detached,   none,   none    >
        >;


    public:
        template <class Fsm,class Event>
        void no_transition(Event const& e, Fsm& ,int state)
        {
            LOGT("no_transition: "<<typeid(e).name()<<", "<<state);
        }

    private:
        Channel _channel;

        Promise< >          _attachPromise {dci::async::PromiseNullInitializer()};
        Promise< Channel>   _detachPromise {dci::async::PromiseNullInitializer()};

    };
}}
