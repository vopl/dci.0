#include <scope.hpp>
#include <cassert>

namespace dci { namespace couple { namespace meta
{
    const std::vector<Type *> &Scope::types() const
    {
        return Compound<Type>::elements();
    }

    const std::vector<Struct *> &Scope::structs() const
    {
        assert(!"filter");
    }

    const std::vector<Variant *> &Scope::variants() const
    {
        assert(!"filter");
    }

    const std::vector<Enum *> &Scope::enums() const
    {
        assert(!"filter");
    }

    const std::vector<Alias *> &Scope::aliases() const
    {
        assert(!"filter");
    }

    const std::vector<Iface *> &Scope::ifaces() const
    {
        assert(!"filter");
    }

    const std::vector<Scope *> &Scope::scopes(bool withDeriveds) const
    {
        if(withDeriveds)
        {
            return Compound<Scope>::elements();
        }

        assert(!"filter deriveds");
    }

}}}
