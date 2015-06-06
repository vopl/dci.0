#include "mutexRecursive.hpp"
#include "waiter.hpp"
#include "scheduler.hpp"

#include <cassert>

namespace dci { namespace async { namespace impl
{
    MutexRecursive::MutexRecursive()
        : Waitable(
              [](Waitable *w){ return static_cast<MutexRecursive*>(w)->canLock();},
              [](Waitable *w){ return static_cast<MutexRecursive*>(w)->tryLock();})
        , _owner(nullptr)
        , _counter(0)
    {
    }

    MutexRecursive::~MutexRecursive()
    {
    }

    void MutexRecursive::lock()
    {
        if(tryLock())
        {
            return;
        }

        assert(_owner && _counter);

        WWLink l;
        l._waitable = this;
        Waiter(&l, 1).any();
    }

    bool MutexRecursive::tryLock()
    {
        if(!_owner)
        {
            assert(!_counter);
            _owner = Scheduler::instance().currentCoro();
            _counter++;
            return true;
        }
        else if(Scheduler::instance().currentCoro() == _owner)
        {
            _counter++;
            return true;
        }

        return false;
    }

    bool MutexRecursive::canLock() const
    {
        assert(!!_owner == !!_counter);

        return !_owner || Scheduler::instance().currentCoro() == _owner;
    }

    void MutexRecursive::unlock()
    {
        assert(_owner && _counter);
        if(!_owner || !_counter)
        {
            abort();
        }

        _counter--;

        if(!_counter)
        {
            _owner = nullptr;

            std::pair<WWLink *, WWLink *> range = _links.range();
            for(WWLink *link = range.first; link!=range.second;)
            {
                WWLink *next = link->_next;
                if(link->_waiter->readyOffer(link))
                {
                    assert(_owner && _counter);
                    return;
                }
                link = next;
            }
        }
    }

}}}
