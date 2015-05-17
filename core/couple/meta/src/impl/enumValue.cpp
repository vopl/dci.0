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

    const std::string &EnumValue::value() const
    {
        return _value;
    }

    void EnumValue::setValue(const std::string &v)
    {
        _value = v;
    }

    void EnumValue::makeSign()
    {
        runtime::SignBuilder s;

        s.add("enumValue");
        s.add(_name);
        s.add("value");
        s.add(_value);

        setSign(s.finish());
    }

}}}}
