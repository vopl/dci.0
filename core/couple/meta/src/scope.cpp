#include <scope.hpp>
#include <cassert>

namespace dci { namespace couple { namespace meta
{
    const std::vector<Struct *> &Scope::structs() const
    {
        return Compound<Struct>::elements();
    }

    const std::vector<Variant *> &Scope::variants() const
    {
        return Compound<Variant>::elements();
    }

    const std::vector<Enum *> &Scope::enums() const
    {
        return Compound<Enum>::elements();
    }

    const std::vector<Alias *> &Scope::aliases() const
    {
        return Compound<Alias>::elements();
    }

    const std::vector<Iface *> &Scope::ifaces() const
    {
        return Compound<Iface>::elements();
    }

    const std::vector<Scope *> &Scope::scopes() const
    {
        return Compound<Scope>::elements();
    }

}}}
