#include <iface.hpp>
#include <cassert>

namespace dci { namespace couple { namespace runtime
{
    Iface::Iface(IfaceState *state, bool fwd)
        : _state(state)
        , _fwd(fwd)
    {
        _state->involve(_fwd, true);
    }

    Iface::Iface(Iface &&from)
        : _state(from._state)
        , _fwd(from._fwd)
    {
        from._state = nullptr;
        //from._fwd = false;
    }

    Iface::~Iface()
    {
        if(_state)
        {
            _state->involve(_fwd, false);
        }
    }

    Iface &Iface::operator=(Iface &&from)
    {
        assert(this != &from);

        if(_state == from.state())
        {
            return *this;
        }

        _state->involve(_fwd, false);
        _state = from._state;
        _fwd = from._fwd;

        from._state = nullptr;
        //from._fwd = false;

        return *this;
    }


    IfaceState *Iface::state()
    {
        assert(_state);
        return _state;
    }

}}}
