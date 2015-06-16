#include "scope.hpp"
#include "struct.hpp"
#include "variant.hpp"
#include "enum.hpp"
#include "alias.hpp"
#include "interface.hpp"
#include <dci/couple/runtime/signBuilder.hpp>

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
        Scoped::makeSign();

        runtime::SignBuilder s;

        s.add("scope");
        s.add(sign());
        s.add(name());

        setSign(s.finish());
    }

}}}}
