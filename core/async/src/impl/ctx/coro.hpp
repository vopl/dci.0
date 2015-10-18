#pragma once

#include "root.hpp"
#include "../task.hpp"
#include <dci/mm/stack.hpp>

namespace dci { namespace async { namespace impl
{
    class Scheduler;

    template <class T>
    class EffortContainer;

}}}

namespace dci { namespace async { namespace impl { namespace ctx
{
    class Coro
        : public Engine
    {
        Coro &operator=(const Coro &) = delete;

        Coro(Scheduler *scheduler, const dci::mm::Stack *stack);
        ~Coro();

    public:
        static Coro *alloc(Scheduler *scheduler);
        void free();

        void setCode(Task *task);

    public:
        void switchTo(Engine *to);

    private:
        [[noreturn]] static void s_contextProc(std::intptr_t self);
        [[noreturn]] void contextProc();

    private:
        Scheduler *             _scheduler;
        const dci::mm::Stack *  _stack;
        Task *                  _task;

    private:
        friend class ::dci::async::impl::EffortContainer<Coro>;
        Coro *  _nextInList;
    };
}}}}
