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
        using Named::name;

        const std::vector<Struct *>     &structs() const;
        const std::vector<Variant *>    &variants() const;
        const std::vector<Enum *>       &enums() const;
        const std::vector<Alias *>      &aliases() const;

        const std::vector<Iface *>      &ifaces() const;

        const std::vector<Scope *>      &scopes() const;

    };

}}}
