#pragma once

#include "named.hpp"
#include "signed.hpp"

namespace dci { namespace couple { namespace meta
{
    class EnumValue
        : public himpl::FaceLayout<impl::EnumValue, Named, Signed>
    {
    public:
        std::uint64_t numericalValue();

    };

}}}
