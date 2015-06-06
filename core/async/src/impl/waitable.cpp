#include "waitable.hpp"

#include <algorithm>
#include <cassert>

namespace dci { namespace async { namespace impl
{

    Waitable::Waitable(bool (* canAcquire)(Waitable *), bool (* tryAcquire)(Waitable *))
        : _links()
        , _canAcquire(canAcquire)
        , _tryAcquire(tryAcquire)
    {
    }

    Waitable::~Waitable()
    {
    }

    void Waitable::wait()
    {
        if(!_tryAcquire(this))
        {
            WWLink l;
            l._waitable = this;
            Waiter(&l, 1).each();
        }
    }

    bool Waitable::canAcquire()
    {
        return _canAcquire(this);
    }

    bool Waitable::tryAcquire()
    {
        return _tryAcquire(this);
    }

    void Waitable::beginAcquire(WWLink *link)
    {
        _links.push(link);
    }

    void Waitable::endAcquire(WWLink *link)
    {
        _links.remove(link);
    }
}}}
