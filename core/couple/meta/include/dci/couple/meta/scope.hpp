#pragma once

#include "named.hpp"
#include "scoped.hpp"
#include "compound.hpp"

namespace dci { namespace couple { namespace meta
{
    class APIDCI_COUPLE_META Scope
        : public himpl::FaceLayout<impl::Scope, Named, Scoped, Compound<Struct>, Compound<Variant>, Compound<Enum>, Compound<Errc>, Compound<Alias>, Compound<Interface>, Compound<Scope>>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Scope() = delete;
        ~Scope() = delete;

        using Named::name;

        const std::vector<const Struct *>     &structs() const;
        const std::vector<const Variant *>    &variants() const;
        const std::vector<const Enum *>       &enums() const;
        const std::vector<const Errc *>       &errcs() const;
        const std::vector<const Alias *>      &aliases() const;

        const std::vector<const Interface *>      &interfaces() const;

        const std::vector<const Scope *>      &scopes() const;

    };

}}}
