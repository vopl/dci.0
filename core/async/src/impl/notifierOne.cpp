#include "notifierOne.hpp"

namespace dci { namespace async { namespace impl
{
    NotifierOne::NotifierOne()
        : Waitable(
              [](Waitable *w){ return static_cast<NotifierOne*>(w)->canAcquire();},
              [](Waitable *w){ return static_cast<NotifierOne*>(w)->tryAcquire();})
        , _ready(false)
    {
    }

    NotifierOne::~NotifierOne()
    {
    }

    void NotifierOne::acquire()
    {
        if(tryAcquire())
        {
            return;
        }

        assert(!_ready);

        WWLink l;
        l._waitable = this;
        Waiter(&l, 1).any();
    }

    bool NotifierOne::tryAcquire()
    {
        if(_ready)
        {
            _ready = false;
            return true;
        }

        return false;
    }

    bool NotifierOne::canAcquire() const
    {
        return _ready;
    }

    void NotifierOne::notify()
    {
        assert(!_ready);

        _ready = true;

        std::pair<WWLink *, WWLink *> range = _links.range();
        for(WWLink *link = range.first; link!=range.second;)
        {
            WWLink *next = link->_next;
            if(link->_waiter->readyOffer(link))
            {
                assert(!_ready);
                return;
            }
            link = next;
        }
    }

}}}
