#include "errc.hpp"
#include <dci/couple/runtime/signBuilder.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    Errc::Errc()
        : Type(TypeConcrete::errc)
    {
    }

    Errc::~Errc()
    {

    }

    void Errc::makeSign()
    {
        Scoped::makeSign();

        runtime::SignBuilder s;

        s.add("errc");
        s.add(sign());
        s.add(_name);

        s.add(Compound<ErrcValue>::elements().size());
        for(auto v : Compound<ErrcValue>::elements())
        {
            s.add(v->sign());
        }

        setSign(s.finish());
    }

}}}}
