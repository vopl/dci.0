#pragma once

#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("../layoutProvider.hpp")
#include <dci/himpl/faceLayout.hpp>

#include "../api.hpp"

namespace dci { namespace async { namespace impl
{
    class Task;
}}}

namespace dci { namespace async { namespace details
{

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    class APIDCI_ASYNC Task
        : public himpl::FaceLayout<impl::Task>
    {
        Task(const Task &) = delete;
        void operator=(const Task &) = delete;

    public:
        using CallExecutor = void (*)(Task *task);
        using FreeExecutor = void (*)(Task *task);

    protected:
        Task(CallExecutor callExecutor, FreeExecutor freeExecutor);
        ~Task();

    public:
        using himpl::FaceLayout<impl::Task>::pimpl;
        using himpl::FaceLayout<impl::Task>::impl;
    };

    APIDCI_ASYNC void spawnTask(details::Task *task);
}}}
