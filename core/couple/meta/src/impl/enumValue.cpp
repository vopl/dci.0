#include "enumValue.hpp"
#include <dci/couple/runtime/signBuilder.hpp>

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
        runtime::SignBuilder s;

        s.add("enumValue");
        s.add(_name);

        setSign(s.finish());
    }

}}}}
