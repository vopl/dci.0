#include "variant.hpp"
#include <dci/couple/runtime/signBuilder.hpp>

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

        runtime::SignBuilder s;

        s.add("variant");
        s.add(sign());

        s.add(Compound<Attribute>::elements().size());
        for(auto v : Compound<Attribute>::elements())
        {
            s.add(v->sign());
        }

        setSign(s.finish());
    }

}}}}
