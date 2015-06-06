#include <dci/async/notifierOne.hpp>
#include "impl/notifierOne.hpp"

namespace dci { namespace async
{
    NotifierOne::NotifierOne()
        : himpl::FaceLayout<impl::NotifierOne, Waitable>()
    {
    }

    NotifierOne::~NotifierOne()
    {
    }

    void NotifierOne::acquire()
    {
        return impl().acquire();
    }

    bool NotifierOne::tryAcquire()
    {
        return impl().tryAcquire();
    }

    bool NotifierOne::canAcquire() const
    {
        return impl().canAcquire();
    }

    void NotifierOne::notify()
    {
        return impl().notify();
    }
}}
