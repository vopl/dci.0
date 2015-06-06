#include "barrier.hpp"

namespace dci { namespace async { namespace impl
{
    Barrier::Barrier(std::size_t depth)
        : Waitable(
            [](Waitable *w){ return static_cast<Barrier*>(w)->canTraverse();},
            [](Waitable *w){ return static_cast<Barrier*>(w)->tryTraverse();})
        , _depth(depth)
        , _amount(0)
    {
    }

    Barrier::~Barrier()
    {
        assert(!_amount);
    }

    void Barrier::traverse()
    {
        if(tryAcquire())
        {
            return;
        }

        assert(_amount < _depth);

        _amount++;
        WWLink l;
        l._waitable = this;
        Waiter(&l, 1).any();
    }

    bool Barrier::tryTraverse()
    {
        assert(_amount < _depth);

        if(canTraverse())
        {
            std::size_t wakedUp(0);
            std::pair<WWLink *, WWLink *> range = _links.range();
            for(WWLink *link = range.first; link!=range.second;)
            {
                WWLink *next = link->_next;
                if(link->_waiter->readyOffer(link))
                {
                    wakedUp++;
                }

                link = next;
            }

            assert(_amount >= wakedUp);
            _amount -= wakedUp;
            assert(_amount < _depth);

            return true;
        }

        return false;
    }

    bool Barrier::canTraverse() const
    {
        return _amount+1 >= _depth;
    }
}}}
