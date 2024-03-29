#include "scheduler.hpp"
#include "ctx/coro.hpp"

#include <algorithm>
#include <cassert>

namespace dci { namespace async { namespace impl
{
    ////////////////////////////////////////////////////////////////////////////////
    Scheduler::Scheduler()
        : _currentCoro(nullptr)
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    Scheduler::~Scheduler()
    {
        ctx::Coro *coro = _emptyCoros.dequeue();
        while(coro)
        {
            coro->free();
            coro = _emptyCoros.dequeue();
        }
    }

    Scheduler &Scheduler::instance()
    {
        return _instance;
    }

    void Scheduler::spawnTask(Task *task)
    {
        _tasks.enqueue(task);
    }

    void Scheduler::yield()
    {
        assert(_currentCoro);
        ctx::Coro *coro = _currentCoro;

        ctx::Coro *next = dequeueReadyCoro();

        if(next && next != coro)
        {
            _readyCoros.enqueue(coro);

            _currentCoro = next;
            coro->switchTo(next);
        }
    }

    void Scheduler::hold()
    {
        assert(_currentCoro);

        switchToNext();
    }

    void Scheduler::ready(ctx::Coro *coro)
    {
        _readyCoros.enqueue(coro);
    }

    bool Scheduler::executeReadyCoros()
    {
        assert(!_currentCoro);

        ctx::Coro *next = dequeueReadyCoro();
        if(next)
        {
            _currentCoro = next;
            _rootContext.switchTo(next);
            return true;
        }

        return false;
    }

    ctx::Coro *Scheduler::currentCoro()
    {
        return _currentCoro;
    }

    void Scheduler::coroCompleted()
    {
        assert(_currentCoro);
        ctx::Coro *coro = _currentCoro;
        _emptyCoros.enqueue(coro);

        switchToNext();
    }

    void Scheduler::switchToNext()
    {
        ctx::Coro *current = _currentCoro;
        ctx::Coro *next = dequeueReadyCoro();
        if(next)
        {
            if(next == current)
            {
                return;
            }

            _currentCoro = next;
            current->switchTo(next);
        }
        else
        {
            _currentCoro = nullptr;
            current->switchTo(&_rootContext);
        }
    }

    ctx::Coro *Scheduler::dequeueReadyCoro()
    {
        ctx::Coro *coro = _readyCoros.dequeue();
        if(coro)
        {
            return coro;
        }

        Task *task = _tasks.dequeue();

        if(task)
        {
            coro = _emptyCoros.dequeue();
            if(!coro)
            {
                coro = ctx::Coro::alloc(this);

                if(!coro)
                {
                    return nullptr;
                }
            }
            coro->setCode(task);
            return coro;
        }

        return nullptr;
    }

    Scheduler Scheduler::_instance{};
}}}
