#include "customIfaceHandler.hpp"


CustomIfaceHandler::CustomIfaceHandler()
{
    f.out1.connect(std::bind(this, &CustomIfaceHandler::onOut1));
    f.out2.connect(std::bind(this, &CustomIfaceHandler::onOut2));
}

CustomIfaceHandler::~CustomIfaceHandler()
{
}

int CustomIfaceHandler::onOut1()
{
    return f.in1();
}

char CustomIfaceHandler::onOut2()
{
    return f.in2();
}
