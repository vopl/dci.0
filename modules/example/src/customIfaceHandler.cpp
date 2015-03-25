#include "customIfaceHandler.hpp"
#include <dci/async/functions.hpp>

#include <functional>

int test()
{

}

CustomIfaceHandler::CustomIfaceHandler()
{
    f.out1.connect(std::bind(&CustomIfaceHandler::onOut1, this));
    f.out2.connect(std::bind(&CustomIfaceHandler::onOut2, this));
    f.out3.connect(std::bind(&CustomIfaceHandler::onOut3, this));
}

CustomIfaceHandler::~CustomIfaceHandler()
{
}

int CustomIfaceHandler::onOut1()
{
    auto r = f.in1();
    if(r.hasError())
    {
        return 0;
    }

    return r.value();
}

dci::async::Future<char> CustomIfaceHandler::onOut2()
{
    return dci::async::spawn([this](dci::async::Promise<char> p) mutable {
        auto r = this->f.in1();
        if(r.hasError())
        {
            p.setValue(0);
            return;
        }

        p.setValue(r.value());
    });
}

CallResult<size_t> CustomIfaceHandler::onOut3()
{
    return f.in3();
}
