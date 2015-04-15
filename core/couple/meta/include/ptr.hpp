#pragma once

#include "signed.hpp"
#include "container1.hpp"

namespace dci { namespace couple { namespace meta
{
    class Ptr
        : public himpl::FaceLayout<impl::Ptr, Signed, Container1>
    {
    public:
        static const TypeConcrete _concrete = TypeConcrete::ptr;

    };

}}}
