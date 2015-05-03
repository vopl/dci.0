#pragma once

#include "named.hpp"
#include "scoped.hpp"
#include "compound.hpp"

namespace dci { namespace couple { namespace meta
{
    class Scope
        : public himpl::FaceLayout<impl::Scope, Named, Scoped, Compound<Struct>, Compound<Variant>, Compound<Enum>, Compound<Alias>, Compound<Iface>, Compound<Scope>>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Scope() = delete;
        ~Scope() = delete;

        using Named::name;

        const std::vector<const Struct *>     &structs() const;
        const std::vector<const Variant *>    &variants() const;
        const std::vector<const Enum *>       &enums() const;
        const std::vector<const Alias *>      &aliases() const;

        const std::vector<const Iface *>      &ifaces() const;

        const std::vector<const Scope *>      &scopes() const;

    };

}}}
