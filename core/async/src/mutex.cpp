#include <dci/async/mutex.hpp>
#include "impl/mutex.hpp"

namespace dci { namespace async
{
    Mutex::Mutex()
        : himpl::FaceLayout<impl::Mutex, Waitable, Lockable>()
    {
    }

    Mutex::~Mutex()
    {
    }

    void Mutex::lock()
    {
        return impl().lock();
    }

    bool Mutex::tryLock()
    {
        return impl().tryLock();
    }

    bool Mutex::canLock() const
    {
        return impl().canLock();
    }

    void Mutex::unlock()
    {
        return impl().unlock();
    }
}}
