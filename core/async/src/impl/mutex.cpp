#include "mutex.hpp"
#include "waiter.hpp"

#include <cassert>

namespace dci { namespace async { namespace impl
{
    Mutex::Mutex()
        : Waitable(
              [](Waitable *w){ return static_cast<Mutex*>(w)->canLock();},
              [](Waitable *w){ return static_cast<Mutex*>(w)->tryLock();})
        , _locked(false)
    {
    }

    Mutex::~Mutex()
    {
    }

    void Mutex::lock()
    {
        if(tryLock())
        {
            return;
        }

        assert(_locked);

        WWLink l;
        l._waitable = this;
        Waiter(&l, 1).any();
    }

    bool Mutex::tryLock()
    {
        if(!_locked)
        {
            _locked = true;
            return true;
        }

        return false;
    }

    bool Mutex::canLock() const
    {
        return !_locked;
    }

    void Mutex::unlock()
    {
        assert(_locked);

        if(!_locked)
        {
            abort();
        }

        _locked = false;

        std::pair<WWLink *, WWLink *> range = _links.range();
        for(WWLink *link = range.first; link!=range.second;)
        {
            WWLink *next = link->_next;
            if(link->_waiter->readyOffer(link))
            {
                assert(_locked);
                return;
            }
            link = next;
        }
    }

}}}
