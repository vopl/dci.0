#pragma once

#include <dci/couple/runtime/iface.hpp>
#include <dci/couple/runtime/callWire.hpp>
#include <dci/couple/runtime/signal.hpp>

#include <dci/async/future.hpp>

using namespace dci::couple::runtime;


class CustomIface1State
    : public dci::couple::runtime::IfaceState
{
public:
    CustomIface1State()
        : dci::couple::runtime::IfaceState([](dci::couple::runtime::IfaceState*state){delete static_cast<CustomIface1State*>(state);})
    {
    }

    CallWire<int()> in1;
    CallWire<char()> in2;
    CallWire<size_t()> in3;

    CallWire<int()> out1;
    CallWire<char()> out2;
    CallWire<size_t()> out3;
};


class CustomIface
    : public dci::couple::runtime::Iface
{
public:
    CustomIface();
    CustomIface(const CustomIface &from);
    CustomIface(CustomIface &&from);
    CustomIface(const Iface &from);
    CustomIface(Iface &&from);
    ~CustomIface();

    CustomIface &operator=(const CustomIface &);
    CustomIface &operator=(CustomIface &&);
    CustomIface &operator=(const Iface &);
    CustomIface &operator=(Iface &&);

public:
    CallResult<int> in1();
    CallResult<char>  in2();
    CallResult<size_t>  in3();

    Signal<int()> out1;
    Signal<char()> out2;
    Signal<size_t()> out3;

private:
    CustomIface1State *state();

};
