#pragma once

#include "task.hpp"
#include "taskPromiseProcessor.hpp"
#include <dci/mm/functions.hpp>

namespace dci { namespace async { namespace details
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    class TaskInstance
        : public Task
    {
    private:
        using PromiseProcessor = TaskPromiseProcessor<F, Args...>;

    public:
        static TaskInstance *alloc(F &&f, Args &&...args);

        using SpawnerResultType = typename PromiseProcessor::SpawnerResultType;
        SpawnerResultType getSpawnerResult();

    private:
        static void callExecutor(Task *task);
        static void freeExecutor(Task *task);

    private:
        TaskInstance(F &&f, Args &&...args);
        ~TaskInstance();

    private:
        typename PromiseProcessor::Call     _call;
        typename PromiseProcessor::Promises _promises;
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    TaskInstance<F, Args...> *TaskInstance<F, Args...>::alloc(F &&f, Args &&...args)
    {
        return new(mm::alloc<sizeof(TaskInstance)>()) TaskInstance(std::forward<F>(f), std::forward<Args>(args)...);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    typename TaskInstance<F, Args...>::SpawnerResultType TaskInstance<F, Args...>::getSpawnerResult()
    {
        return PromiseProcessor::getSpawnerResult(_promises);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    void TaskInstance<F, Args...>::callExecutor(Task *task)
    {
        TaskInstance *self = static_cast<TaskInstance*>(task);

        return PromiseProcessor::doCall(self->_call, self->_promises);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    void TaskInstance<F, Args...>::freeExecutor(Task *task)
    {
        TaskInstance *self = static_cast<TaskInstance*>(task);

        self->~TaskInstance();

        mm::free<sizeof(TaskInstance)>(self);
    }

    namespace
    {
        template <class F>
        auto forwardTaskInstanceFunctor(std::enable_if_t<!std::is_member_function_pointer<F>::value, F> &f) -> F &&
        {
            return std::forward<F>(f);
        }

        template <class F>
        auto forwardTaskInstanceFunctor(std::enable_if_t<std::is_member_function_pointer<F>::value, F> &f) -> decltype(std::mem_fn(f))
        {
            return std::mem_fn(std::forward<F>(f));
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    TaskInstance<F, Args...>::TaskInstance(F &&f, Args &&...args)
        : Task(&TaskInstance::callExecutor, &TaskInstance::freeExecutor)
        , _call(forwardTaskInstanceFunctor<F>(f), std::forward<Args>(args)...)
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    TaskInstance<F, Args...>::~TaskInstance()
    {
    }

}}}
