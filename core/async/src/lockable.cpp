#include <dci/async/lockable.hpp>
#include "impl/lockable.hpp"

namespace dci { namespace async
{
    Lockable::Lockable()
        : himpl::FaceLayout<impl::Lockable>()
    {
    }

    Lockable::~Lockable()
    {
    }

    void Lockable::lock()
    {
        return impl().lock();
    }

    void Lockable::unlock()
    {
        return impl().unlock();
    }

}}
