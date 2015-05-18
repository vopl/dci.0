#include "engine.hpp"
#include "epoll.hpp"
#include <dci/poller/descriptor.hpp>
#include <dci/poller/error.hpp>

namespace dci { namespace poller { namespace impl
{
    Engine::Engine()
    {
    }

    Engine::~Engine()
    {
        while(_descriptors)
        {
            _descriptors->close();
            _descriptors->addReadyState(poller::Descriptor::rsf_error);
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

    std::error_code Engine::deinitialize()
    {
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
