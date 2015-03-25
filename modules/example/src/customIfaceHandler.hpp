#pragma once

#include "customIface.hpp"

#include <functional>

class CustomIfaceHandler
{
    CustomIface f;

public:
    CustomIfaceHandler();
    ~CustomIfaceHandler();

    int onOut1();
    dci::async::Future<char> onOut2();
    CallResult<size_t> onOut3();

};
