#include "scope.hpp"
#include "struct.hpp"
#include "variant.hpp"
#include "enum.hpp"
#include "alias.hpp"
#include "iface.hpp"
#include "signBuilder.hpp"

#include <cassert>

namespace dci { namespace couple { namespace meta { namespace impl
{
    Scope::Scope()
    {

    }

    Scope::~Scope()
    {

    }

    void Scope::makeSign()
    {
        SignBuilder s;

        s.add("scope");
        s.add(name());

        setSign(s.finish());
    }

}}}}
