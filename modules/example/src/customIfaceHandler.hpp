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
    char onOut2();

};