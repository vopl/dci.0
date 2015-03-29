#pragma once

#include <dci/couple/runtime/iface.hpp>
#include <dci/couple/runtime/call/wire.hpp>
#include <dci/couple/runtime/call/signal.hpp>

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

    call::Wire<int()> in1;
    call::Wire<char()> in2;
    call::Wire<size_t()> in3;

    call::Wire<int()> out1;
    call::Wire<char()> out2;
    call::Wire<size_t()> out3;
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
    call::Future<int> in1();
    call::Future<char> in2();
    call::Future<size_t> in3();

    call::Signal<int()> out1;
    call::Signal<char()> out2;
    call::Signal<size_t()> out3;

private:
    CustomIface1State *state();

};
