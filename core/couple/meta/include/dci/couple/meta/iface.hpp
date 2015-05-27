#pragma once

#include "type.hpp"
#include "scope.hpp"
#include "compound.hpp"
#include "method.hpp"
#include "inheritable.hpp"

namespace dci { namespace couple { namespace meta
{
    class APIDCI_COUPLE_META Iface
        : public himpl::FaceLayout<impl::Iface, Type, Scope, Compound<Method>, Inheritable<Iface>>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Iface() = delete;
        ~Iface() = delete;

        static const TypeConcrete _concrete = TypeConcrete::iface;

        const std::vector<const Method *> &methods() const;

        bool primary() const;
        const Iface *opposite() const;
    };

}}}
