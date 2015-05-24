#pragma once

#include <dci/poll/timer.hpp>

namespace dci { namespace poll { namespace impl
{
    namespace timerEngine
    {
        class GridPoint;
        struct TimerState
        {
            TimerState(TimerState *next = nullptr, TimerState *prev = nullptr);

            TimerState *_next;
            TimerState *_prev;

            GridPoint *_gridPoint;
        };
    }

    class Timer
        : public timerEngine::TimerState
    {

    public:
        Timer(
                std::chrono::milliseconds interval,
                bool repeatable);

        ~Timer();

        std::chrono::milliseconds interval() const;
        void interval(std::chrono::milliseconds v);

        bool repeatable() const;
        void repeatable(bool v);

        void start();
        bool started() const;
        void stop();

        async::Pulser &pulser();

    public:
        std::chrono::steady_clock::time_point bound() const;
        void tick(std::chrono::steady_clock::time_point now);

    private:
        bool                        _started;
        bool                        _repeatable;
        std::chrono::nanoseconds   _interval;

        async::Pulser               _pulser;

        std::chrono::steady_clock::time_point   _bound;
    };
}}}