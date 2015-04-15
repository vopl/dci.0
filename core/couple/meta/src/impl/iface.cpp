#include "iface.hpp"
#include "signBuilder.hpp"

#include <iostream>

namespace dci { namespace couple { namespace meta { namespace impl
{
    Iface::Iface()
        : Type(TypeConcrete::iface)
    {
    }

    Iface::~Iface()
    {

    }

    void Iface::makeSign()
    {
        Scope::makeSign();

        SignBuilder s;

        s.add("iface");
        s.add(sign());

        s.add(Compound<Method>::elements().size());
        for(auto v : Compound<Method>::elements())
        {
            s.add(v->sign());
        }

        s.add(Inheritable<Iface>::bases().size());
        for(auto v : Inheritable<Iface>::bases())
        {
            s.add(v->sign());
        }

        setSign(s.finish());
    }

}}}}
