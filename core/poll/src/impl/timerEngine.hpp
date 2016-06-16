#pragma once
#include "timer.hpp"
#include "timerEngine/grid.hpp"
#include <dci/logger.hpp>

namespace dci { namespace poll { namespace impl
{
    class TimerEngine
    {
    public:
        TimerEngine();
        ~TimerEngine();

    public:
        std::chrono::milliseconds fireTicks();

    public:
        void update(Timer *t);

        void start(Timer *t);
        void stop(Timer *t);

        void flush(Timer *t, std::size_t fromLevel);



    private:
        using LevelsUnit = std::chrono::duration<int64_t, std::ratio<1, 10000>>;

        std::chrono::steady_clock::time_point   _now;

        timerEngine::time                       _lastCheckpoint;
        std::chrono::milliseconds               _lastDistance;
        timerEngine::Grid<4, 16>                _grid;

    };
}}}
