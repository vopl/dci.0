#pragma once

#include "container1.hpp"

namespace dci { namespace couple { namespace meta
{
    class List
        : public himpl::FaceLayout<impl::List, Container1>
    {
    public:
        static const TypeConcrete _concrete = TypeConcrete::list;

    };

}}}
