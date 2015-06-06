#include "semaphore.hpp"

namespace dci { namespace async { namespace impl
{
    Semaphore::Semaphore(std::size_t depth)
        : Waitable(
              [](Waitable *w){ return static_cast<Semaphore*>(w)->canLock();},
              [](Waitable *w){ return static_cast<Semaphore*>(w)->tryLock();})
        , _depth(depth)
    {
    }

    Semaphore::~Semaphore()
    {
    }

    void Semaphore::lock()
    {
        if(tryLock())
        {
            return;
        }

        assert(!_depth);

        WWLink l;
        l._waitable = this;
        Waiter(&l, 1).any();
    }

    bool Semaphore::tryLock()
    {
        if(_depth > 0)
        {
            _depth--;
            return true;
        }

        return false;
    }

    bool Semaphore::canLock() const
    {
        return _depth > 0;
    }

    void Semaphore::unlock()
    {
        _depth++;

        std::pair<WWLink *, WWLink *> range = _links.range();
        for(WWLink *link = range.first; link!=range.second;)
        {
            WWLink *next = link->_next;
            if(link->_waiter->readyOffer(link))
            {
                if(!_depth)
                {
                    return;
                }
            }
            link = next;
        }
    }

}}}
