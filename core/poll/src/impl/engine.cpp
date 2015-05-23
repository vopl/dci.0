#include "engine.hpp"
#include "epoll.hpp"
#include <dci/poll/descriptor.hpp>
#include <dci/poll/error.hpp>

namespace dci { namespace poll { namespace impl
{
    Engine::Engine()
    {
    }

    Engine::~Engine()
    {
        while(_descriptors)
        {
            _descriptors->close(false);
            _descriptors->addReadyState(poll::Descriptor::rsf_error);
            _descriptors = _descriptors->_nextInEngine;
        }
    }

    std::error_code Engine::initialize()
    {
        assert(!engine);
        if(engine)
        {
            return make_error_code(error::general::already_initialized);
        }

        engine.reset(new Epoll);

        std::error_code ec = engine->startup();

        if(ec)
        {
            engine.reset();
            return ec;
        }

        return std::error_code();
    }

    std::error_code Engine::run()
    {
        if(!_stop)
        {
            return make_error_code(error::general::already_started);
        }

        _stop = false;

        std::chrono::milliseconds timeout;
        while(!_stop)
        {
            do
            {
                timeout = _timerEngine.fireTicks();
            }
            while(async::executeReadyCoros());

            auto ec = execute(timeout);
            if(ec)
            {
                return ec;
            }
        }
        async::executeReadyCoros();

        return std::error_code();
    }

    std::error_code Engine::stop()
    {
        _stop = true;
        return interrupt();
    }

    TimerEngine &Engine::timerEngine()
    {
        return _timerEngine;
    }

    std::error_code Engine::deinitialize()
    {
        if(!_stop)
        {
            return make_error_code(error::general::not_stopped);
        }

        assert(engine);
        if(!engine)
        {
            return make_error_code(error::general::not_initialized);
        }

        engine.reset();

        return std::error_code();
    }

    void Engine::addReadyState(Descriptor *d, std::uint_fast32_t state)
    {
        d->addReadyState(state);
    }

    void Engine::registerDescriptor(Descriptor *d)
    {
        assert(!d->_nextInEngine);
        assert(!d->_prevInEngine);
        if(!_descriptors)
        {
            _descriptors = d;
        }
        else
        {
            assert(!_descriptors->_prevInEngine);
            _descriptors->_prevInEngine = d;
            d->_nextInEngine = _descriptors;
            _descriptors = d;
        }
    }

    bool Engine::deregisterDescriptor(Descriptor *d)
    {
        if(!d->_nextInEngine && !d->_prevInEngine && d != _descriptors)
        {
            return false;
        }

        if(d == _descriptors)
        {
            if(d->_prevInEngine)
            {
                _descriptors = d->_prevInEngine;
            }
            else
            {
                _descriptors = d->_nextInEngine;
            }
        }

        if(d->_nextInEngine)
        {
            d->_nextInEngine->_prevInEngine = d->_prevInEngine;
        }
        if(d->_prevInEngine)
        {
            d->_prevInEngine->_nextInEngine = d->_nextInEngine;
        }

        d->_nextInEngine = nullptr;
        d->_prevInEngine = nullptr;

        return true;
    }

    std::unique_ptr<Engine> engine;
}}}
