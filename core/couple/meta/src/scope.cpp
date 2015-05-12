#include <dci/couple/meta/scope.hpp>
#include "impl/scope.hpp"
#include <cassert>

namespace dci { namespace couple { namespace meta
{
    const std::vector<const Struct *> &Scope::structs() const
    {
        return Compound<Struct>::elements();
    }

    const std::vector<const Variant *> &Scope::variants() const
    {
        return Compound<Variant>::elements();
    }

    const std::vector<const Enum *> &Scope::enums() const
    {
        return Compound<Enum>::elements();
    }

    const std::vector<const Errc *> &Scope::errcs() const
    {
        return Compound<Errc>::elements();
    }

    const std::vector<const Alias *> &Scope::aliases() const
    {
        return Compound<Alias>::elements();
    }

    const std::vector<const Iface *> &Scope::ifaces() const
    {
        return Compound<Iface>::elements();
    }

    const std::vector<const Scope *> &Scope::scopes() const
    {
        return Compound<Scope>::elements();
    }

}}}
