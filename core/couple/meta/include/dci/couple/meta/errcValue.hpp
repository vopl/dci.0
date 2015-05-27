#pragma once

#include "named.hpp"
#include "signed.hpp"

namespace dci { namespace couple { namespace meta
{
    class APIDCI_COUPLE_META ErrcValue
        : public himpl::FaceLayout<impl::ErrcValue, Named, Signed>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        ErrcValue() = delete;
        ~ErrcValue() = delete;

        const std::string &description() const;

    };

}}}
