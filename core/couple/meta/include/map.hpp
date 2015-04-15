#pragma once

#include "signed.hpp"
#include "container2.hpp"

namespace dci { namespace couple { namespace meta
{
    class Map
        : public himpl::FaceLayout<impl::Map, Signed, Container2>
    {
    public:
        static const TypeConcrete _concrete = TypeConcrete::map;

    };

}}}
