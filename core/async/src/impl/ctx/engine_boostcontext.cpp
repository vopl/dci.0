#include "engine_boostcontext.hpp"
#include "dci/mm/config.hpp"

#include <cstddef>
#include <cassert>

namespace dci { namespace async { namespace impl { namespace ctx
{
    ////////////////////////////////////////////////////////////////////////////////
    boost::context::fcontext_t Engine::_rootCtx;

    ////////////////////////////////////////////////////////////////////////////////
    Engine::Engine()
        : _ctxPtr()
        , _coroArg()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::constructRoot()
    {
        assert(!_ctxPtr);
        assert(&_rootCtx != _ctxPtr);
        assert(!_coroArg);

        _ctxPtr = &_rootCtx;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::destructRoot()
    {
        //assert(_ctxPtr);
        assert(&_rootCtx == _ctxPtr);
        assert(!_coroArg);

        _ctxPtr = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::constructCoro(char *stackPtr, std::size_t stackSize, void(*f)(std::intptr_t), std::uintptr_t arg)
    {
        assert(!_ctxPtr);
        assert(&_rootCtx != _ctxPtr);
        assert(!_coroArg);

        _coroArg = arg;

        if(mm::Config::_stackGrowsDown)
        {
            _ctxPtr = boost::context::make_fcontext(
                      stackPtr + stackSize,
                      stackSize,
                      f);
        }
        else
        {
            _ctxPtr = boost::context::make_fcontext(
                      stackPtr,
                      stackSize,
                      f);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::destructCoro()
    {
        assert(_ctxPtr);
        assert(&_rootCtx != _ctxPtr);

        _ctxPtr = nullptr;
        _coroArg = 0;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::switchTo(Engine *to)
    {
        boost::context::jump_fcontext(_ctxPtr, to->_ctxPtr, to->_coroArg, false);
    }
}}}}
