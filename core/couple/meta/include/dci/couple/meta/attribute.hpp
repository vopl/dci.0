#pragma once

#include "named.hpp"
#include "signed.hpp"

namespace dci { namespace couple { namespace meta
{
    class APIDCI_COUPLE_META Attribute
        : public himpl::FaceLayout<impl::Attribute, Named, Signed>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Attribute() = delete;
        ~Attribute() = delete;

        const Type *type() const;

    };

}}}
