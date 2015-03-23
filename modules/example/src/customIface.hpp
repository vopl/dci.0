#pragma once

#include <dci/couple/runtime/iface.hpp>
#include <dci/couple/runtime/callWire.hpp>
#include <dci/couple/runtime/signal.hpp>

using namespace dci::couple::runtime;


class CustomIface1State
{
    CallWire<int()> in1;
    CallWire<char()> in2;

    CallWire<int()> out1;
    CallWire<char()> out2;
};


class CustomIface
    : public dci::couple::runtime::Iface
{
public:
    CustomIface();
    CustomIface(const CustomIface &);
    CustomIface(const Iface &);
    ~CustomIface();

    CustomIface &operator=(const CustomIface &);
    CustomIface &operator=(const Iface &);

public:
    int in1();
    char in2();

    Signal<int()> out1;
    Signal<char()> out2;

};
