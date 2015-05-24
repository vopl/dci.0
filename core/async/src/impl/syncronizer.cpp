#include "syncronizer.hpp"

#include <algorithm>
#include <cassert>

namespace dci { namespace async { namespace impl
{

    Syncronizer::Syncronizer()
        : _waiters()
    {
    }

    Syncronizer::~Syncronizer()
    {
    }

    bool Syncronizer::registerWaiter(SyncronizerWaiter *waiter, std::size_t data)
    {
        _waiters.emplace_back(waiter, data);
        return !locked();
    }

    void Syncronizer::unregisterWaiterAndCommitAcquire(SyncronizerWaiter *waiter, std::size_t data)
    {
        unregisterWaiter(waiter, data);

        assert(!locked());
        lock();
    }

    void Syncronizer::unregisterWaiter(SyncronizerWaiter *waiter, std::size_t data)
    {
        WaiterWithData syncronizerWaiterWithData {waiter, data};
        TVWaiters::reverse_iterator iter = std::find(_waiters.rbegin(), _waiters.rend(), syncronizerWaiterWithData);
        assert(_waiters.rend() != iter);
        _waiters.erase((iter+1).base());
    }

    void Syncronizer::unlock()
    {
        std::size_t idx(0);
        while(idx < _waiters.size() && !locked())
        {
            WaiterWithData &waiterWithData = _waiters[idx];
            if(waiterWithData._waiter->released(waiterWithData._data))
            {
                //was removed
            }
            else
            {
                idx++;
            }
        }
    }

    Syncronizer::WaiterWithData::WaiterWithData(SyncronizerWaiter *waiter, std::size_t data)
        : _waiter(waiter)
        , _data(data)
    {
    }

    bool Syncronizer::WaiterWithData::operator==(const WaiterWithData &with) const
    {
        return _waiter == with._waiter &&
                _data == with._data;
    }

}}}
