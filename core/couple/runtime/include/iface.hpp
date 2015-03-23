#pragma once

#include "ifaceId.hpp"
#include "ifaceState.hpp"

namespace dci { namespace couple { namespace runtime
{
    class Iface
    {
    public:
        Iface();
        ~Iface();

    private:
        IfaceStatePtr _state;
    };




//    class CustomIface1Handler
//    {
//        CustomIface1 f;
//    public:
//        CustomIface1Handler()
//        {
//            f.out1.connect(&onOut1);
//            f.out2.connect(&onOut2);
//        }

//        ~CustomIface1Handler()
//        {
//            //f.out1.disconnect();
//            //f.out2.disconnect();
//        }

//        int onOut1()
//        {
//            return f.in1();
//        }
//        char onOut2()
//        {
//            return f.in2();
//        }
//    };

}}}
