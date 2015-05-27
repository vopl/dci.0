#pragma once

#include "details/taskInstance.hpp"
#include "api.hpp"

namespace dci { namespace async
{
    APIDCI_ASYNC void yield();

    APIDCI_ASYNC bool executeReadyCoros();

    ////////////////////////////////////////////////////////////////////////////////
    template<class F, class... Args>
    typename details::TaskInstance<F, Args...>::SpawnerResultType spawn(F&& f, Args&&... args)
    {
        auto task = details::TaskInstance<F, Args...>::alloc(std::forward<F>(f), std::forward<Args>(args)...);
        spawnTask(task);
        return task->getSpawnerResult();
    }

}}
