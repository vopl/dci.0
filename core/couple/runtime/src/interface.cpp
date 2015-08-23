#include <dci/couple/runtime/interface.hpp>
#include <cassert>

namespace dci { namespace couple { namespace runtime
{
    Interface::Interface(InterfaceWires *state, bool fwd)
        : _wires(state)
        , _fwd(fwd)
    {
        if(_wires)
        {
            _wires->involve(_fwd, true);
        }
    }

    Interface::Interface(Interface &&from)
        : _wires(from._wires)
        , _fwd(from._fwd)
    {
        if(_wires)
        {
            from._wires = nullptr;
            //from._fwd = false;
        }
    }

    Interface::~Interface()
    {
        if(_wires)
        {
            _wires->involve(_fwd, false);
        }
    }

    Interface &Interface::operator=(Interface &&from)
    {
        assert(this != &from);

        if(_wires == from.wires())
        {
            return *this;
        }

        if(_wires)
        {
            _wires->involve(_fwd, false);
        }
        _wires = from._wires;
        _fwd = from._fwd;

        from._wires = nullptr;
        //from._fwd = false;

        if(_wires)
        {
            assert(_wires->involved(_fwd));
        }

        return *this;
    }

    void Interface::assign(InterfaceWires *wire, bool fwd)
    {
        if(_wires == wire)
        {
            assert(_fwd == fwd);
            return;
        }

        if(_wires)
        {
            _wires->involve(_fwd, false);
        }
        _wires = wire;
        _fwd = fwd;
        if(_wires)
        {
            _wires->involve(_fwd, true);
        }
    }

    InterfaceWires *Interface::wires()
    {
        //assert(_wires);
        return _wires;
    }

    bool Interface::fwd() const
    {
        return _fwd;
    }

    bool Interface::oppositeInvolved() const
    {
        assert(_wires);
        return _wires->involved(!_fwd);
    }

    Interface::operator bool() const
    {
        return !!_wires;
    }

    bool Interface::operator !() const
    {
        return !_wires;
    }

    Interface::Interface(NullInterfaceInitializer)
        : _wires(nullptr)
        , _fwd(true)
    {
    }


}}}
