#pragma once

#include "signed.hpp"
#include "container1.hpp"

namespace dci { namespace couple { namespace meta
{
    class Set
        : public himpl::FaceLayout<impl::Set, Signed, Container1>
    {
    public:
        static const TypeConcrete _concrete = TypeConcrete::set;

    };

}}}
