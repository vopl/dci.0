#pragma once

#include "type.hpp"

namespace dci { namespace couple { namespace meta
{
    class APIDCI_COUPLE_META Container1
        : public himpl::FaceLayout<impl::Container1, Type>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Container1() = delete;
        ~Container1() = delete;

        const Type *elementType() const;
    };

}}}
