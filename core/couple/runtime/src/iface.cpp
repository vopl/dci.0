#include <iface.hpp>
#include <cassert>

namespace dci { namespace couple { namespace runtime
{
    Iface::Iface(IfaceState *state)
        : _state(state)
    {
        _state->incRef();
    }

    Iface::Iface(const Iface &from)
        : _state(from._state)
    {
        _state->incRef();
    }

    Iface::Iface(Iface &&from)
        : _state(from._state)
    {
        from._state = nullptr;
    }

    Iface::~Iface()
    {
        if(_state)
        {
            _state->decRef();
        }
    }

    Iface &Iface::operator=(const Iface &from)
    {
        _state->decRef();
        _state = from._state;
        _state->incRef();
        return *this;
    }

    Iface &Iface::operator=(Iface &&from)
    {
        _state->decRef();
        _state = from._state;
        from._state = nullptr;
        return *this;
    }


    IfaceState *Iface::state()
    {
        assert(_state);
        return _state;
    }

}}}
