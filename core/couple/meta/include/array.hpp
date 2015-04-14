#pragma once

#include "container1.hpp"

#include <cstdint>

namespace dci { namespace couple { namespace meta
{
    class Array
        : public himpl::FaceLayout<impl::Array, Container1>
    {
    public:
        static const TypeConcrete _concrete = TypeConcrete::array;

        std::uint32_t size() const;

    };

}}}
