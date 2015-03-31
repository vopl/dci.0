#include "customIfaceHandler.hpp"
#include <dci/async/functions.hpp>

#include <functional>

int test()
{

}

CustomIfaceHandler::CustomIfaceHandler()
{
    f.out1.connect(&CustomIfaceHandler::onOut1, this);
    f.out2.connect(&CustomIfaceHandler::onOut2, this);
    f.out3.connect(&CustomIfaceHandler::onOut3, this);
}

CustomIfaceHandler::~CustomIfaceHandler()
{
}

dci::async::Future<call::Error, int> CustomIfaceHandler::onOut1()
{
    return f.in1();
}

dci::async::Future<call::Error, char> CustomIfaceHandler::onOut2()
{
    assert(0);
//    return dci::async::spawn([this](dci::async::Promise<char> p) mutable {
//        auto r = this->f.in1();
//        if(r.hasError())
//        {
//            p.setValue(0);
//            return;
//        }

//        p.setValue(r.value());
//    });
}

dci::async::Future<call::Error, size_t> CustomIfaceHandler::onOut3(double d)
{
    return f.in3(d);
}
