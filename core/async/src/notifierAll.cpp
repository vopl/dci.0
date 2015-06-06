#include <dci/async/notifierAll.hpp>
#include "impl/notifierAll.hpp"

namespace dci { namespace async
{
    NotifierAll::NotifierAll()
        : himpl::FaceLayout<impl::NotifierAll, Waitable>()
    {
    }

    NotifierAll::~NotifierAll()
    {
    }

    void NotifierAll::acquire()
    {
        return impl().acquire();
    }

    bool NotifierAll::tryAcquire()
    {
        return impl().tryAcquire();
    }

    bool NotifierAll::canAcquire() const
    {
        return impl().canAcquire();
    }

    void NotifierAll::notify()
    {
        return impl().notify();
    }
}}
