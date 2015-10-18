#include <dci/couple/runtime/interface.hpp>
#include <cassert>

namespace dci { namespace couple { namespace runtime
{
    Interface::Interface(InterfaceDirection direction)
        : _direction(direction)
        , _wires()
    {
    }

    Interface::Interface(InterfaceDirection direction, InterfaceWires *wires)
        : _direction(direction)
        , _wires(wires)
    {
        assert(_wires);
        assert(!_wires->involved(_direction));
        _wires->involve(_direction, true);
    }

    Interface::Interface(Interface &&from)
        : _direction(from.direction())
        , _wires(from._wires)
    {
        if(_wires)
        {
            from._wires = nullptr;
        }
    }

    Interface::Interface(InterfaceOpposite &from)
        : _direction(InterfaceDirection::fwd == from.direction() ? InterfaceDirection::bwd : InterfaceDirection::fwd)
        , _wires(from._wires)
    {
        assert(_wires);
        assert(!_wires->involved(_direction));
        _wires->involve(_direction, true);
    }

    Interface::~Interface()
    {
        if(_wires)
        {
            _wires->involve(_direction, false);
        }
    }

    Interface &Interface::operator=(Interface &&from)
    {
        assert(_direction == from._direction);
        if(_direction != from._direction)
        {
            abort();
        }

        assert(this != &from);

        if(_wires == from.wires())
        {
            return *this;
        }

        if(_wires)
        {
            _wires->involve(_direction, false);
        }

        _wires = from._wires;
        from._wires = nullptr;

        if(_wires)
        {
            assert(_wires->involved(_direction));
        }

        return *this;
    }

    Interface &Interface::operator=(InterfaceOpposite &from)
    {
        assert(_direction != from._direction);
        if(_direction == from._direction)
        {
            abort();
        }

        assert(this != &from);

        if(_wires == from.wires())
        {
            return *this;
        }

        if(_wires)
        {
            _wires->involve(_direction, false);
        }

        _wires = from._wires;

        assert(_wires);
        if(!_wires)
        {
            abort();
        }

        _wires->involve(_direction, true);

        return *this;

    }

    void Interface::assignWires(InterfaceWires *wires)
    {
        if(_wires == wires)
        {
            return;
        }

        if(_wires)
        {
            _wires->involve(_direction, false);
        }

        _wires = wires;

        assert(_wires);
        if(!_wires)
        {
            abort();
        }

        _wires->involve(_direction, true);
    }

    void Interface::virtualAssignOpposite(InterfaceOpposite &from)
    {
        (void)from;
        assert(0&&"must be overrided");
    }

    InterfaceDirection Interface::direction() const
    {
        return _direction;
    }

    InterfaceWires *Interface::wires()
    {
        return _wires;
    }

    Interface::operator bool() const
    {
        return !!_wires;
    }

    bool Interface::operator !() const
    {
        return !_wires;
    }

}}}
