#include "enumValue.hpp"
#include "signBuilder.hpp"

#include <cassert>

namespace dci { namespace couple { namespace meta { namespace impl
{
    EnumValue::EnumValue()
    {

    }

    EnumValue::~EnumValue()
    {

    }

    void EnumValue::makeSign()
    {
        SignBuilder s;

        s.add("enumValue");
        s.add(_name);

        setSign(s.finish());
    }

}}}}
