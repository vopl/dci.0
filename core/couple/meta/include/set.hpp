#pragma once

#include "container1.hpp"

namespace dci { namespace couple { namespace meta
{
    class Set
        : public himpl::FaceLayout<impl::Set, Container1>
    {
    public:
        static const TypeConcrete _concrete = TypeConcrete::set;

    };

}}}
