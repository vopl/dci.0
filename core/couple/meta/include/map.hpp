#pragma once

#include "container2.hpp"

namespace dci { namespace couple { namespace meta
{
    class Map
        : public himpl::FaceLayout<impl::Map, Container2>
    {
    public:
        static const TypeConcrete _concrete = TypeConcrete::map;

    };

}}}
