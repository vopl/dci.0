#include <dci/async/semaphore.hpp>
#include "impl/semaphore.hpp"
#include "impl/lockable.hpp"

namespace dci { namespace async
{
    Semaphore::Semaphore(std::size_t depth)
        : himpl::FaceLayout<impl::Semaphore, Waitable, Lockable>(depth)
    {
    }

    Semaphore::~Semaphore()
    {
    }

    void Semaphore::lock()
    {
        return impl().lock();
    }

    bool Semaphore::tryLock()
    {
        return impl().tryLock();
    }

    bool Semaphore::canLock() const
    {
        return impl().canLock();
    }

    void Semaphore::unlock()
    {
        return impl().unlock();
    }
}}
