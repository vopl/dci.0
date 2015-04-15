#include "set.hpp"
#include "signBuilder.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    Set::Set()
        : Container1(TypeConcrete::set)
    {
    }

    Set::~Set()
    {

    }

    void Set::makeSign()
    {
        SignBuilder s;

        s.add("set");
        s.add(_elementType->concreteSign());

        setSign(s.finish());
    }

}}}}
