#pragma once

#include "customIface.hpp"

#include <functional>

class CustomIfaceHandler
{
public:
    CustomIface f;

public:
    CustomIfaceHandler();
    ~CustomIfaceHandler();

    dci::async::Future<call::Error, call::Value<int>> onOut1();
    dci::async::Future<call::Error, call::Value<char>> onOut2();
    dci::async::Future<call::Error, call::Value<size_t>> onOut3(call::Value<double> d);

};
