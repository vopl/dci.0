#include <dci/couple/runtime/interface.hpp>
#include <cassert>

namespace dci { namespace couple { namespace runtime
{
    Interface::Interface(InterfaceWire *state, bool fwd)
        : _wire(state)
        , _fwd(fwd)
    {
        _wire->involve(_fwd, true);
    }

    Interface::Interface(Interface &&from)
        : _wire(from._wire)
        , _fwd(from._fwd)
    {
        from._wire = nullptr;
        //from._fwd = false;
    }

    Interface::~Interface()
    {
        if(_wire)
        {
            _wire->involve(_fwd, false);
        }
    }

    Interface &Interface::operator=(Interface &&from)
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

    void Interface::assign(InterfaceWire *wire, bool fwd)
    {
        if(_wire == wire)
        {
            assert(_fwd == fwd);
            return;
        }

        _wire->involve(_fwd, false);
        _wire = wire;
        _fwd = fwd;
        _wire->involve(_fwd, true);
    }

    InterfaceWire *Interface::wire()
    {
        assert(_wire);
        return _wire;
    }

}}}
