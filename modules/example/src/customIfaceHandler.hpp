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

    call::Future<int> onOut1();
    call::Future<char> onOut2();
    call::Future<size_t> onOut3(double d);

};
