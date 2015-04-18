#pragma once

#include "type.hpp"
#include "named.hpp"
#include "scoped.hpp"
#include "compound.hpp"
#include "enumValue.hpp"

namespace dci { namespace couple { namespace meta
{
    class Enum
        : public himpl::FaceLayout<impl::Enum, Type, Named, Scoped, Compound<EnumValue>>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Enum() = delete;
        ~Enum() = delete;

        static const TypeConcrete _concrete = TypeConcrete::enum_;

        const std::vector<const EnumValue *> &values() const;

    };

}}}
