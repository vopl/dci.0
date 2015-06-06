#include "pulserOne.hpp"

namespace dci { namespace async { namespace impl
{
    PulserOne::PulserOne()
        : Waitable(
              [](Waitable *w){ return static_cast<PulserOne*>(w)->canAcquire();},
              [](Waitable *w){ return static_cast<PulserOne*>(w)->tryAcquire();})
        , _ready(false)
    {
    }

    PulserOne::~PulserOne()
    {
    }

    void PulserOne::acquire()
    {
        assert(!_ready);

        WWLink l;
        l._waitable = this;
        Waiter(&l, 1).any();
    }

    bool PulserOne::tryAcquire()
    {
        if(_ready)
        {
            _ready = false;
            return true;
        }

        return false;
    }

    bool PulserOne::canAcquire() const
    {
        return _ready;
    }

    void PulserOne::pulse()
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

        _ready = false;
    }

}}}
