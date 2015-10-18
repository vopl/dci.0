#include "waiter.hpp"
#include "waitable.hpp"
#include "scheduler.hpp"

namespace dci { namespace async { namespace impl
{
    ////////////////////////////////////////////////////////////////////////////////
    Waiter::Waiter(WWLink *links, std::size_t amount)
        : _links(links)
        , _amount(amount)
        , _mode(Mode::null)
        , _perModeState()
        , _coro(nullptr)
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    Waiter::~Waiter()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    std::size_t Waiter::any()
    {
        assert(Mode::null == _mode);
        assert(!_coro);
        assert(!_perModeState._acquiredIndex);

        _mode = Mode::any;

        WWLink *linksEnd = _links + _amount;
        for(WWLink *link = _links; link != linksEnd; ++link)
        {
            if(link->_waitable->tryAcquire())
            {
                for(WWLink *link2 = _links; link2 != link; ++link2)
                {
                    link2->_waitable->endAcquire(link2);
                }
                return static_cast<std::size_t>(link - _links);
            }
            else
            {
                link->_waiter = this;
                link->_waitable->beginAcquire(link);
            }
        }

        _coro = Scheduler::instance().currentCoro();
        Scheduler::instance().hold();

        return _perModeState._acquiredIndex;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Waiter::all()
    {
        assert(Mode::null == _mode);
        assert(!_coro);

        _mode = Mode::all;

        bool allReady = true;

        WWLink *linksEnd = _links + _amount;
        for(WWLink *link = _links; link != linksEnd; ++link)
        {
            if(!link->_waitable->canAcquire())
            {
                allReady = false;
                break;
            }
        }

        if(allReady)
        {
            for(WWLink *link = _links; link != linksEnd; ++link)
            {
                bool b = link->_waitable->tryAcquire();
                assert(b);
                (void)b;
            }

            return;
        }

        for(WWLink *link = _links; link != linksEnd; ++link)
        {
            link->_waiter = this;
            link->_waitable->beginAcquire(link);
        }

        _coro = Scheduler::instance().currentCoro();
        Scheduler::instance().hold();
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Waiter::each()
    {
        assert(Mode::null == _mode);
        assert(!_coro);
        assert(!_perModeState._acquiredAmount);

        _mode = Mode::each;

        WWLink *linksEnd = _links + _amount;
        for(WWLink *link = _links; link != linksEnd; ++link)
        {
            if(link->_waitable->tryAcquire())
            {
                _perModeState._acquiredAmount++;
            }
            else
            {
                link->_waiter = this;
                link->_waitable->beginAcquire(link);
            }
        }

        if(_perModeState._acquiredAmount < _amount)
        {
            _coro = Scheduler::instance().currentCoro();
            Scheduler::instance().hold();
        }

        assert(_perModeState._acquiredAmount == _amount);
    }

    ////////////////////////////////////////////////////////////////////////////////
    bool Waiter::readyOffer(WWLink *link)
    {
        assert(link >= _links && link < _links+_amount);

        switch(_mode)
        {
        case Mode::null:
            return true;

        case Mode::any:
            {
                WWLink *linksEnd = _links + _amount;
                for(WWLink *iter = _links; iter != linksEnd; ++iter)
                {
                    iter->_waitable->endAcquire(iter);
                }
                _perModeState._acquiredIndex = static_cast<std::size_t>(link - _links);
                Scheduler::instance().ready(_coro);
            }
            return true;

        case Mode::all:
            {
                WWLink *linksEnd = _links + _amount;
                for(WWLink *iter = _links; iter != linksEnd; ++iter)
                {
                    if(iter == link)
                    {
                        continue;
                    }

                    if(!iter->_waitable->canAcquire())
                    {
                        return false;
                    }
                }
                for(WWLink *iter = _links; iter != linksEnd; ++iter)
                {
                    iter->_waitable->endAcquire(iter);
                    if(iter == link)
                    {
                        continue;
                    }

                    bool b = iter->_waitable->tryAcquire();
                    assert(b);
                    (void)b;
                }
                Scheduler::instance().ready(_coro);
            }
            return true;

        case Mode::each:
            {
                link->_waitable->endAcquire(link);

                _perModeState._acquiredAmount++;
                if(_perModeState._acquiredAmount == _amount)
                {
                    Scheduler::instance().ready(_coro);
                }
            }
            return true;

        default:
            break;
        }

        assert(0&&"never here");
        abort();
    }
}}}
