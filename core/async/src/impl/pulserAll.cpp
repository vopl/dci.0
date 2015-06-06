#include "pulserAll.hpp"

namespace dci { namespace async { namespace impl
{
    PulserAll::PulserAll()
        : Waitable(
              [](Waitable *w){ return static_cast<PulserAll*>(w)->canAcquire();},
              [](Waitable *w){ return static_cast<PulserAll*>(w)->tryAcquire();})
        , _ready(false)
    {
    }

    PulserAll::~PulserAll()
    {
    }

    void PulserAll::acquire()
    {
        assert(!_ready);

        WWLink l;
        l._waitable = this;
        Waiter(&l, 1).any();
    }

    bool PulserAll::tryAcquire()
    {
        return _ready;
    }

    bool PulserAll::canAcquire() const
    {
        return _ready;
    }

    void PulserAll::pulse()
    {
        assert(!_ready);

        _ready = true;

        std::pair<WWLink *, WWLink *> range = _links.range();
        for(WWLink *link = range.first; link!=range.second;)
        {
            WWLink *next = link->_next;
            link->_waiter->readyOffer(link);
            link = next;
        }

        _ready = false;
    }

}}}
