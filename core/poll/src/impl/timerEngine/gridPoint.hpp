#pragma once
#include "../timer.hpp"

namespace dci { namespace poll { namespace impl
{
    class TimerEngine;
}}}

namespace dci { namespace poll { namespace impl { namespace timerEngine
{
    struct GridPoint
    {
        void timersAmount(std::size_t *v);
        void insert(Timer *v);
        void remove(Timer *v);
        void flush(TimerEngine *engine, std::size_t fromLevel);

        bool empty() const;

    private:
        TimerState _begin   {&_end, nullptr};
        TimerState _end     {nullptr, &_begin};

        std::size_t *_timersAmount  {nullptr};
    };
}}}}
