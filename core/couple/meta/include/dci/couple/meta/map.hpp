#pragma once

#include "signed.hpp"
#include "container2.hpp"

namespace dci { namespace couple { namespace meta
{
    class APIDCI_COUPLE_META Map
        : public himpl::FaceLayout<impl::Map, Signed, Container2>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Map() = delete;
        ~Map() = delete;

        static const TypeConcrete _concrete = TypeConcrete::map;

    };

}}}
