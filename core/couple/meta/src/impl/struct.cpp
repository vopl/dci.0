#include "struct.hpp"
#include "signBuilder.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    Struct::Struct()
        : Type(TypeConcrete::struct_)
    {
    }

    Struct::~Struct()
    {

    }

    void Struct::makeSign()
    {
        Scope::makeSign();

        SignBuilder s;

        s.add("struct");
        s.add(sign());

        s.add(Compound<Attribute>::elements().size());
        for(auto v : Compound<Attribute>::elements())
        {
            s.add(v->sign());
        }

        s.add(Inheritable<Struct>::bases().size());
        for(auto v : Inheritable<Struct>::bases())
        {
            s.add(v->sign());
        }

        setSign(s.finish());
    }

}}}}
