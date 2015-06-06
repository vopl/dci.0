#include <dci/async/event.hpp>
#include "impl/event.hpp"
#include "impl/waitable.hpp"

namespace dci { namespace async
{
    Event::Event()
        : himpl::FaceLayout<impl::Event, Waitable>()
    {
    }

    Event::~Event()
    {
    }

    void Event::acquire()
    {
        return impl().acquire();
    }

    bool Event::tryAcquire()
    {
        return impl().tryAcquire();
    }
    bool Event::canAcquire() const
    {
        return impl().canAcquire();
    }

    void Event::set()
    {
        return impl().set();
    }

    void Event::reset()
    {
        return impl().reset();
    }

}}
