#pragma once

#include "details/taskInstance.hpp"

namespace dci { namespace async
{
    void yield();

    bool executeReadyCoros();

    ////////////////////////////////////////////////////////////////////////////////
    template<class F, class... Args>
    typename details::TaskInstance<F, Args...>::SpawnerResultType spawn(F&& f, Args&&... args)
    {
        auto task = details::TaskInstance<F, Args...>::alloc(std::forward<F>(f), std::forward<Args>(args)...);
        spawnTask(task);
        return task->getSpawnerResult();
    }

}}
