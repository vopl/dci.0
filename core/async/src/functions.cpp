#include <dci/async/functions.hpp>
#include "impl/scheduler.hpp"

namespace dci { namespace async
{
    ////////////////////////////////////////////////////////////////////////////////
    void yield()
    {
        return impl::Scheduler::instance().yield();
    }

    ////////////////////////////////////////////////////////////////////////////////
    bool executeReadyCoros()
    {
        return impl::Scheduler::instance().executeReadyCoros();
    }

}}
