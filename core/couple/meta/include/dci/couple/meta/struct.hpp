#pragma once

#include "type.hpp"
#include "scope.hpp"
#include "compound.hpp"
#include "attribute.hpp"
#include "inheritable.hpp"

namespace dci { namespace couple { namespace meta
{
    class Struct
        : public himpl::FaceLayout<impl::Struct, Type, Scope, Compound<Attribute>, Inheritable<Struct>>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Struct() = delete;
        ~Struct() = delete;

        static const TypeConcrete _concrete = TypeConcrete::struct_;

        const std::vector<const Attribute *> &fields() const;


    };

}}}
