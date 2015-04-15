#include "ptr.hpp"
#include "signBuilder.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    Ptr::Ptr()
        : Container1(TypeConcrete::ptr)
    {
    }

    Ptr::~Ptr()
    {

    }

    void Ptr::makeSign()
    {
        SignBuilder s;

        s.add("ptr");
        s.add(_elementType->concreteSign());

        setSign(s.finish());
    }

}}}}
