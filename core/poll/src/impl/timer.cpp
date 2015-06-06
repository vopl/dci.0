#include "timer.hpp"
#include "engine.hpp"
#include <dci/logger.hpp>

namespace dci { namespace poll { namespace impl
{
    namespace timerEngine
    {
        TimerState::TimerState(TimerState *next, TimerState *prev)
            : _next(next)
            , _prev(prev)
            , _gridPoint()
        {
        }
    }

    Timer::Timer(
            std::chrono::milliseconds interval,
            bool repeatable)
        : _started(false)
        , _repeatable(repeatable)
        , _interval(interval)
    {
    }

    Timer::~Timer()
    {
        stop();
    }

    std::chrono::milliseconds Timer::interval() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(_interval);
    }

    void Timer::interval(std::chrono::milliseconds v)
    {
        if(started())
        {
            if(_interval.count() > 0)
            {
                if(v.count() > 0)
                {
                    _bound = std::chrono::steady_clock::now()+v;
                    engine->timerEngine().update(this);
                }
                else
                {
                    engine->timerEngine().stop(this);
                }
            }
            else
            {
                if(v.count() > 0)
                {
                    _bound = std::chrono::steady_clock::now()+v;
                    engine->timerEngine().start(this);
                }
                else
                {
                    //nothing
                }
            }
        }

        _interval = v;
    }

    bool Timer::repeatable() const
    {
        return _repeatable;
    }

    void Timer::repeatable(bool v)
    {
        _repeatable = v;
    }

    void Timer::start()
    {
        assert(0 < _interval.count());
        if(!_started)
        {
            if(_interval.count() > 0)
            {
                _bound = std::chrono::steady_clock::now()+_interval;
                engine->timerEngine().start(this);
            }

            _started = true;
        }
    }

    bool Timer::started() const
    {
        return _started;
    }

    void Timer::stop()
    {
        if(_started)
        {
            if(_interval.count() > 0)
            {
                engine->timerEngine().stop(this);
            }
            _started = false;
        }
    }

    async::PulserAll &Timer::pulser()
    {
        return _pulser;
    }

    std::chrono::steady_clock::time_point Timer::bound() const
    {
        return _bound;
    }

    void Timer::tick(std::chrono::steady_clock::time_point now)
    {
        assert(_started);
        assert(0 < _interval.count());

        if(_repeatable)
        {
            if(now < _bound)
            {
                _bound += _interval;
                assert(_bound > now);
            }
            else
            {
                auto b = _bound + (now-_bound)/_interval*_interval;
                if(b > now)
                {
                    _bound = b;
                }
                else
                {
                    _bound = b+_interval;
                }
                assert(_bound > now);
            }

            engine->timerEngine().start(this);
            _pulser.pulse();
        }
        else
        {
            _started = false;
        }
    }

}}}
