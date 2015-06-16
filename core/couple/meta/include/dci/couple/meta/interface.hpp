#pragma once

#include "type.hpp"
#include "scope.hpp"
#include "compound.hpp"
#include "method.hpp"
#include "inheritable.hpp"

namespace dci { namespace couple { namespace meta
{
    class APIDCI_COUPLE_META Interface
        : public himpl::FaceLayout<impl::Interface, Type, Scope, Compound<Method>, Inheritable<Interface>>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Interface() = delete;
        ~Interface() = delete;

        static const TypeConcrete _concrete = TypeConcrete::interface;

        const std::vector<const Method *> &methods() const;

        bool primary() const;
        const Interface *opposite() const;
    };

}}}
