#include "interface.hpp"
#include <dci/couple/runtime/signBuilder.hpp>

#include <iostream>

namespace dci { namespace couple { namespace meta { namespace impl
{
    Interface::Interface()
        : Type(TypeConcrete::interface)
    {
    }

    Interface::~Interface()
    {

    }

    void Interface::makeSign()
    {
        Scope::makeSign();

        runtime::SignBuilder s;

        s.add("interface");
        s.add(sign());

        s.add(Compound<Method>::elements().size());
        for(auto v : Compound<Method>::elements())
        {
            s.add(v->sign());
        }

        s.add(Inheritable<Interface>::bases().size());
        for(auto v : Inheritable<Interface>::bases())
        {
            s.add(v->sign());
        }

        setSign(s.finish());
    }

    void Interface::setPrimary(bool v)
    {
        _primary = v;
    }

    bool Interface::primary() const
    {
        return _primary;
    }

    void Interface::setOpposite(Interface *v)
    {
        _opposite = v;
    }

    const Interface *Interface::opposite() const
    {
        return _opposite;
    }

}}}}
