#pragma once

#include "type.hpp"

namespace dci { namespace couple { namespace meta
{
    class APIDCI_COUPLE_META Container2
        : public himpl::FaceLayout<impl::Container2, Type>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Container2() = delete;
        ~Container2() = delete;

        const Type *elementType1() const;
        const Type *elementType2() const;
    };

}}}
