#include "set.hpp"
#include <dci/couple/runtime/signBuilder.hpp>

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
        runtime::SignBuilder s;

        s.add("set");
        s.add(_elementType->concreteSign());

        setSign(s.finish());
    }

}}}}
