#include "notifierAll.hpp"

namespace dci { namespace async { namespace impl
{
    NotifierAll::NotifierAll()
        : Waitable(
              [](Waitable *w){ return static_cast<NotifierAll*>(w)->canAcquire();},
              [](Waitable *w){ return static_cast<NotifierAll*>(w)->tryAcquire();})
        , _ready(false)
    {
    }

    NotifierAll::~NotifierAll()
    {
    }

    void NotifierAll::acquire()
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

    bool NotifierAll::tryAcquire()
    {
        if(_ready)
        {
            _ready = false;
            return true;
        }

        return false;
    }

    bool NotifierAll::canAcquire() const
    {
        return _ready;
    }

    void NotifierAll::notify()
    {
        assert(!_ready);

        _ready = true;

        bool notifiedAtLeastOne = false;

        std::pair<WWLink *, WWLink *> range = _links.range();
        for(WWLink *link = range.first; link!=range.second;)
        {
            WWLink *next = link->_next;
            if(link->_waiter->readyOffer(link))
            {
                notifiedAtLeastOne = true;
            }
            link = next;
        }

        if(notifiedAtLeastOne)
        {
            _ready = false;
        }
    }

}}}
