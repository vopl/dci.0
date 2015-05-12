#include "errcValue.hpp"
#include <dci/couple/runtime/signBuilder.hpp>

#include <cassert>

namespace dci { namespace couple { namespace meta { namespace impl
{
    ErrcValue::ErrcValue()
    {

    }

    ErrcValue::~ErrcValue()
    {

    }

    const std::string &ErrcValue::description() const
    {
        return _description;
    }

    void ErrcValue::setDescription(const std::string &v)
    {
        _description = v;
    }

    void ErrcValue::makeSign()
    {
        runtime::SignBuilder s;

        s.add("errcValue");
        s.add(_name);
        s.add("description");
        s.add(_description);

        setSign(s.finish());
    }

}}}}
