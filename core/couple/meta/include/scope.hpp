#pragma once

#include "named.hpp"
#include "type.hpp"
#include "compound.hpp"

namespace dci { namespace couple { namespace meta
{
    class Scope
        : public himpl::FaceLayout<impl::Scope, Named, Compound<Type>, Compound<Scope>>
    {
    public:
        using Named::name;

        const std::vector<Type *>       &types() const;

        const std::vector<Struct *>     &structs() const;
        const std::vector<Variant *>    &variants() const;
        const std::vector<Enum *>       &enums() const;
        const std::vector<Alias *>      &aliases() const;

        const std::vector<Iface *>      &ifaces() const;

        const std::vector<Scope *>      &scopes(bool withDeriveds = false) const;

    };

}}}
