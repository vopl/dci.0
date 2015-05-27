#pragma once

#include "signed.hpp"
#include "container1.hpp"

#include <cstdint>

namespace dci { namespace couple { namespace meta
{
    class APIDCI_COUPLE_META Array
        : public himpl::FaceLayout<impl::Array, Signed, Container1>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Array() = delete;
        ~Array() = delete;

        static const TypeConcrete _concrete = TypeConcrete::array;

        std::uint32_t size() const;

    };

}}}
