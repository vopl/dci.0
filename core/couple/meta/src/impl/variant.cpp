#include "variant.hpp"
#include "signBuilder.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    Variant::Variant()
        : Type(TypeConcrete::variant)
    {
    }

    Variant::~Variant()
    {

    }

    void Variant::makeSign()
    {
        Scope::makeSign();

        SignBuilder s;

        s.add("variant");
        s.add(sign());

        s.add(Compound<Attribute>::elements().size());
        for(auto v : Compound<Attribute>::elements())
        {
            s.add(v->sign());
        }

        s.add(Inheritable<Variant>::bases().size());
        for(auto v : Inheritable<Variant>::bases())
        {
            s.add(v->sign());
        }

        setSign(s.finish());
    }

}}}}
