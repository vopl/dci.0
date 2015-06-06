#include "event.hpp"

namespace dci { namespace async { namespace impl
{
    Event::Event()
        : Waitable(
            [](Waitable *w){ return static_cast<Event*>(w)->canAcquire();},
            [](Waitable *w){ return static_cast<Event*>(w)->tryAcquire();})
        , _ready(false)
    {
    }

    Event::~Event()
    {
    }

    void Event::acquire()
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

    bool Event::tryAcquire()
    {
        return _ready;
    }

    bool Event::canAcquire() const
    {
        return _ready;
    }

    void Event::set()
    {
        if(!_ready)
        {
            _ready = true;

            std::pair<WWLink *, WWLink *> range = _links.range();
            for(WWLink *link = range.first; link!=range.second;)
            {
                WWLink *next = link->_next;
                link->_waiter->readyOffer(link);
                link = next;
            }
        }
    }

    void Event::reset()
    {
        _ready = false;
    }

}}}
