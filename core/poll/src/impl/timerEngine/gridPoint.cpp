#include "gridPoint.hpp"
#include "../timerEngine.hpp"

namespace dci { namespace poll { namespace impl { namespace timerEngine
{

    void GridPoint::timersAmount(std::size_t *v)
    {
        _timersAmount = v;
    }

    void GridPoint::insert(Timer *v)
    {
        assert(!v->_next);
        assert(!v->_prev);
        assert(!v->_gridPoint);

        v->_next = _begin._next;
        _begin._next->_prev = v;


        v->_prev = &_begin;
        _begin._next = v;

        v->_gridPoint = this;

        (*_timersAmount) ++;
    }

    void GridPoint::remove(Timer *v)
    {
        assert(this == v->_gridPoint);
        assert(v->_next);
        assert(v->_prev);

        v->_next->_prev = v->_prev;
        v->_prev->_next = v->_next;

        v->_next = nullptr;
        v->_prev = nullptr;

        v->_gridPoint = nullptr;

        (*_timersAmount) --;
    }

    void GridPoint::flush(TimerEngine *engine, std::size_t fromLevel)
    {
        TimerState *iter = _begin._next;
        if(iter != &_end)
        {
            _begin._next->_prev = nullptr;
            _end._prev->_next = nullptr;

            _begin._next = &_end;
            _end._prev = &_begin;

            while(iter)
            {
                Timer *t = static_cast<Timer *>(iter);
                iter = iter->_next;

                if(t->_next)
                {
                    t->_next->_prev = nullptr;
                    t->_next = nullptr;
                }

                if(t->_prev)
                {
                    t->_prev = nullptr;
                    t->_prev->_next = nullptr;
                }

                assert(this == t->_gridPoint);
                t->_gridPoint = nullptr;

                (*_timersAmount) --;

                engine->flush(t, fromLevel);
            }
        }
    }

    bool GridPoint::empty() const
    {
        return _begin._next == &_end;
    }

}}}}
