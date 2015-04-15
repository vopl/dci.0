#include "list.hpp"
#include "signBuilder.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    List::List()
        : Container1(TypeConcrete::list)
    {
    }

    List::~List()
    {

    }

    void List::makeSign()
    {
        SignBuilder s;

        s.add("list");
        s.add(_elementType->concreteSign());

        setSign(s.finish());
    }

}}}}
