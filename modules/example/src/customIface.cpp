#include "customIfaceHandler.hpp"
#include <cassert>

using namespace dci::couple::runtime;

CustomIface::CustomIface()
    : Iface(new CustomIface1State)
{
}

CustomIface::CustomIface(CustomIface &&from)
    : Iface(std::forward<Iface>(from))
{
}


CustomIface::CustomIface(Iface &&from)
    : Iface(std::forward<Iface>(from))
{
    //TODO: validate cast
}

CustomIface::~CustomIface()
{
}


CustomIface &CustomIface::operator=(CustomIface &&from)
{
    Iface::operator=(std::move(from));
    return *this;
}

CustomIface &CustomIface::operator=(Iface &&from)
{
    Iface::operator=(std::move(from));
    return *this;
}

dci::async::Future<call::Error, int> CustomIface::in1()
{
    return state()->in1();
}

dci::async::Future<call::Error, char> CustomIface::in2()
{
    return state()->in2();
}

dci::async::Future<call::Error, size_t> CustomIface::in3(double d)
{
    return state()->in3(std::move(d));
}

CustomIface1State *CustomIface::state()
{
    return static_cast<CustomIface1State *>(Iface::state());
}
