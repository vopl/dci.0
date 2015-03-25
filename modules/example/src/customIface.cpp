#include "customIfaceHandler.hpp"
#include <cassert>

using namespace dci::couple::runtime;

CustomIface::CustomIface()
    : Iface(new CustomIface1State)
{
}

CustomIface::CustomIface(const CustomIface &from)
    : Iface(from)
{
}

CustomIface::CustomIface(CustomIface &&from)
    : Iface(std::forward<Iface>(from))
{
}


CustomIface::CustomIface(const Iface &from)
    : Iface(from)
{
    //TODO: validate cast
}

CustomIface::CustomIface(Iface &&from)
    : Iface(std::forward<Iface>(from))
{
    //TODO: validate cast
}

CustomIface::~CustomIface()
{
}


CustomIface &CustomIface::operator=(const CustomIface &from)
{
    Iface::operator=(from);
    return *this;
}

CustomIface &CustomIface::operator=(CustomIface &&from)
{
    Iface::operator=(std::move(from));
    return *this;
}

CustomIface &CustomIface::operator=(const Iface &from)
{
    Iface::operator=(from);
    return *this;
}

CustomIface &CustomIface::operator=(Iface &&from)
{
    Iface::operator=(std::move(from));
    return *this;
}

CallResult<int> CustomIface::in1()
{
    return state()->in1();
}

CallResult<char> CustomIface::in2()
{
    return state()->in2();
}

CallResult<size_t> CustomIface::in3()
{
    return state()->in3();
}

CustomIface1State *CustomIface::state()
{
    return static_cast<CustomIface1State *>(Iface::state());
}
