#include "enum.hpp"
#include <dci/couple/runtime/signBuilder.hpp>

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
        runtime::SignBuilder s;

        s.add("enum");
        s.add(sign());
        s.add(_name);

        s.add(Compound<EnumValue>::elements().size());
        for(auto v : Compound<EnumValue>::elements())
        {
            s.add(v->sign());
        }

        setSign(s.finish());
    }

}}}}
