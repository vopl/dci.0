#pragma once

#include "signed.hpp"
#include "container1.hpp"

namespace dci { namespace couple { namespace meta
{
    class APIDCI_COUPLE_META Ptr
        : public himpl::FaceLayout<impl::Ptr, Signed, Container1>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Ptr() = delete;
        ~Ptr() = delete;

        static const TypeConcrete _concrete = TypeConcrete::ptr;

    };

}}}
