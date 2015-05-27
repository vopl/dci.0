#pragma once

#include "named.hpp"
#include "signed.hpp"

namespace dci { namespace couple { namespace meta
{
    class APIDCI_COUPLE_META EnumValue
        : public himpl::FaceLayout<impl::EnumValue, Named, Signed>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        EnumValue() = delete;
        ~EnumValue() = delete;

        const std::string &value() const;

    };

}}}
