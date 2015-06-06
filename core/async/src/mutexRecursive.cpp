#include <dci/async/mutexRecursive.hpp>
#include "impl/mutexRecursive.hpp"

namespace dci { namespace async
{
    MutexRecursive::MutexRecursive()
        : himpl::FaceLayout<impl::MutexRecursive, Waitable, Lockable>()
    {
    }

    MutexRecursive::~MutexRecursive()
    {
    }

    void MutexRecursive::lock()
    {
        return impl().lock();
    }

    bool MutexRecursive::tryLock()
    {
        return impl().tryLock();
    }

    bool MutexRecursive::canLock() const
    {
        return impl().canLock();
    }

    void MutexRecursive::unlock()
    {
        return impl().unlock();
    }
}}
