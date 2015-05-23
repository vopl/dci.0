#include "timerEngine.hpp"
#include <dci/logger.hpp>

namespace dci { namespace poll { namespace impl
{
    using namespace timerEngine;

    TimerEngine::TimerEngine()
        : _now(std::chrono::steady_clock::now())
        , _grid(std::chrono::duration_cast<LevelsUnit>(_now.time_since_epoch()).count())
    {

    }

    TimerEngine::~TimerEngine()
    {

    }

    std::chrono::milliseconds TimerEngine::fireTicks()
    {
        _now = std::chrono::steady_clock::now();

        time now = std::chrono::duration_cast<LevelsUnit>(_now.time_since_epoch()).count();
        time next = _grid.rollTo(now, this);
        if(_maxtime == next)
        {
            return std::chrono::milliseconds::max();
        }

        return LevelsUnit(next - now);
    }

    void TimerEngine::update(Timer *t)
    {
        _grid.replace(t, std::chrono::duration_cast<LevelsUnit>(t->bound().time_since_epoch()).count());
    }

    void TimerEngine::start(Timer *t)
    {
        _grid.insert(t, std::chrono::duration_cast<LevelsUnit>(t->bound().time_since_epoch()).count());
    }

    void TimerEngine::stop(Timer *t)
    {
        _grid.remove(t);
    }

    void TimerEngine::flush(Timer *t, std::size_t fromLevel)
    {
        if(!fromLevel || t->bound() <= _now)
        {
            t->tick(_now);
        }
        else
        {
            _grid.insert(t, std::chrono::duration_cast<LevelsUnit>(t->bound().time_since_epoch()).count());
        }
    }
}}}
