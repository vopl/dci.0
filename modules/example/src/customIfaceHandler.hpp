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
    call::Future<size_t> onOut3();

};
