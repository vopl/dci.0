#include <iface.hpp>
#include <cassert>

namespace dci { namespace couple { namespace runtime
{
    Iface::Iface(IfaceWire *state, bool fwd)
        : _wire(state)
        , _fwd(fwd)
    {
        _wire->involve(_fwd, true);
    }

    Iface::Iface(Iface &&from)
        : _wire(from._wire)
        , _fwd(from._fwd)
    {
        from._wire = nullptr;
        //from._fwd = false;
    }

    Iface::~Iface()
    {
        if(_wire)
        {
            _wire->involve(_fwd, false);
        }
    }

    Iface &Iface::operator=(Iface &&from)
    {
        assert(this != &from);

        if(_wire == from.wire())
        {
            return *this;
        }

        _wire->involve(_fwd, false);
        _wire = from._wire;
        _fwd = from._fwd;

        from._wire = nullptr;
        //from._fwd = false;

        return *this;
    }


    IfaceWire *Iface::wire()
    {
        assert(_wire);
        return _wire;
    }

}}}
