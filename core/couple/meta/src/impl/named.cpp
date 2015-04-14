#include "named.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    Named::Named()
    {

    }

    Named::~Named()
    {

    }

    const std::string &Named::name() const
    {
        return _name;
    }

    void Named::setName(const std::string &name)
    {
        _name = name;
    }

}}}}
