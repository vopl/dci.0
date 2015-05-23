#pragma once
#include "gridPoint.hpp"
#include <dci/logger.hpp>

namespace dci { namespace poll { namespace impl
{
    class TimerEngine;
}}}

namespace dci { namespace poll { namespace impl { namespace timerEngine
{
    using time = std::uint64_t;
    static constexpr time _maxtime = -1;

    template <int volumeBits, int ordersAmount, int order=0>
    struct Grid
    {
        Grid(time m);
        ~Grid();

        time rollTo(time m, TimerEngine *engine);

        time nextTimeWithPayload() const;
        time nextTimeWithPayloadFirst() const;

        void insert(Timer *v, time m);
        void replace(Timer *v, time m);
        void remove(Timer *v);

    private:
        static const std::size_t _pointsAmount = 1 << volumeBits;
        static const time _precisionGranula = time(1) << (volumeBits*order);
        static const time _precisionMask = ~(_precisionGranula-1);

        GridPoint   _points[_pointsAmount];
        time        _currentTime;
        time        _nextTimeWithPayload;
        std::size_t _timersAmount;

        Grid<volumeBits, ordersAmount, order+1> _nextLevel;
    };

    template <int volumeBits, int order>
    struct Grid<volumeBits, order, order>
    {
        Grid(time){}
        void insert(Timer *, time) {}
        time rollTo(time, TimerEngine *) {return _maxtime;}
        time nextTimeWithPayload() const {return _maxtime;}
        time nextTimeWithPayloadFirst() const {return _maxtime;}
    };

















    template <int volumeBits, int ordersAmount, int order>
    Grid<volumeBits, ordersAmount, order>::Grid(time m)
        : _currentTime(m & _precisionMask)
        , _nextTimeWithPayload(_maxtime)
        , _timersAmount(0)
        , _nextLevel(_currentTime + _precisionGranula*_pointsAmount)
    {
        for(GridPoint &gp : _points)
        {
            gp.timersAmount(&_timersAmount);
        }
    }


    template <int volumeBits, int ordersAmount, int order>
    Grid<volumeBits, ordersAmount, order>::~Grid()
    {
    }

    template <int volumeBits, int ordersAmount, int order>
    time Grid<volumeBits, ordersAmount, order>::rollTo(time m, TimerEngine *engine)
    {
        if(_currentTime <= m)
        {
            std::size_t pointIndex = _currentTime / _precisionGranula;
            std::size_t pointIndexEndRange = m / _precisionGranula + 1;
            std::size_t pointIndexEnd = pointIndex + _pointsAmount;

            _nextTimeWithPayload = _maxtime;

            while(pointIndex < pointIndexEndRange && _timersAmount)
            {
                assert(pointIndex < pointIndexEnd);
                _points[pointIndex % _pointsAmount].flush(engine, order);
                pointIndex++;
            }

            if(_timersAmount)
            {
                while(pointIndex < pointIndexEnd)
                {
                    if(!_points[pointIndex % _pointsAmount].empty())
                    {
                        _nextTimeWithPayload = pointIndex * _precisionGranula;
                        break;
                    }

                    pointIndex++;
                }

                assert(_maxtime != _nextTimeWithPayload);
            }

            _currentTime = (m & _precisionMask) + _precisionGranula;

            return std::min(_nextLevel.rollTo(m, engine), _nextTimeWithPayload);
        }

        return nextTimeWithPayload();
    }

    template <int volumeBits, int ordersAmount, int order>
    time Grid<volumeBits, ordersAmount, order>::nextTimeWithPayload() const
    {
        if(_maxtime != _nextTimeWithPayload)
        {
            return std::min(_nextLevel.nextTimeWithPayloadFirst(), _nextTimeWithPayload);
        }
        return _nextLevel.nextTimeWithPayload();
    }

    template <int volumeBits, int ordersAmount, int order>
    time Grid<volumeBits, ordersAmount, order>::nextTimeWithPayloadFirst() const
    {
        if(_currentTime == _nextTimeWithPayload)
        {
            return _nextTimeWithPayload;
        }

        return _maxtime;
    }

    template <int volumeBits, int ordersAmount, int order>
    void Grid<volumeBits, ordersAmount, order>::insert(Timer *v, time m)
    {
        if(m <= _currentTime+_precisionGranula*_pointsAmount)
        {
            if(m < _currentTime)
            {
                m = _currentTime;
            }

            std::size_t pointIndex = (m / _precisionGranula) % _pointsAmount;
            _points[pointIndex].insert(v);

            _nextTimeWithPayload = std::min(m & _precisionMask, _nextTimeWithPayload);
            return;
        }

        _nextLevel.insert(v, m);
    }

    template <int volumeBits, int ordersAmount, int order>
    void Grid<volumeBits, ordersAmount, order>::replace(Timer *v, time m)
    {
        remove(v);
        insert(v, m);
    }

    template <int volumeBits, int ordersAmount, int order>
    void Grid<volumeBits, ordersAmount, order>::remove(Timer *v)
    {
        assert(v->_gridPoint);
        v->_gridPoint->remove(v);
    }
}}}}
