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

    dci::async::Future<call::Error, int> onOut1();
    dci::async::Future<call::Error, char> onOut2();
    dci::async::Future<call::Error, size_t> onOut3(double d);

};
