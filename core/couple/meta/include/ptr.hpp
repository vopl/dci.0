#pragma once

#include "container1.hpp"

namespace dci { namespace couple { namespace meta
{
    class Ptr
        : public himpl::FaceLayout<impl::Ptr, Container1>
    {
    public:
        static const TypeConcrete _concrete = TypeConcrete::ptr;

    };

}}}
