#pragma once

#include "type.hpp"
#include "scope.hpp"
#include "compound.hpp"
#include "attribute.hpp"

namespace dci { namespace couple { namespace meta
{
    class Variant
        : public himpl::FaceLayout<impl::Variant, Type, Scope, Compound<Attribute>>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Variant() = delete;
        ~Variant() = delete;

        static const TypeConcrete _concrete = TypeConcrete::variant;

        const std::vector<const Attribute *> &fields() const;

    };

}}}
