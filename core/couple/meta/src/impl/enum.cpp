#include "enum.hpp"
#include "signBuilder.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    Enum::Enum()
        : Type(TypeConcrete::enum_)
    {
    }

    Enum::~Enum()
    {

    }

    void Enum::makeSign()
    {
        SignBuilder s;

        s.add("enum");
        s.add(scopesSign());
        s.add(_name);

        s.add(Compound<EnumValue>::elements().size());
        for(auto v : Compound<EnumValue>::elements())
        {
            s.add(v->sign());
        }

        s.add(Inheritable<Enum>::bases().size());
        for(auto v : Inheritable<Enum>::bases())
        {
            s.add(v->sign());
        }

        setSign(s.finish());
    }

}}}}
