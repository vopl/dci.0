#pragma once

#include "type.hpp"
#include "named.hpp"
#include "scoped.hpp"
#include "compound.hpp"
#include "enumValue.hpp"
#include "inheritable.hpp"

namespace dci { namespace couple { namespace meta
{
    class Enum
        : public himpl::FaceLayout<impl::Enum, Type, Named, Scoped, Compound<EnumValue>, Inheritable<Enum>>
    {
    public:
        static const TypeConcrete _concrete = TypeConcrete::enum_;

        std::vector<EnumValue *> values();

        std::uint32_t bitness();


    };

}}}
