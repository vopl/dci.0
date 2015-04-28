#include "scoped.hpp"
#include "scope.hpp"
#include <dci/couple/runtime/signBuilder.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    Scoped::Scoped()
    {
    }

    Scoped::~Scoped()
    {
    }

    const Scope *Scoped::scope() const
    {
        return _scope;
    }

    void Scoped::setScope(Scope *scope)
    {
        _scope = scope;
    }

    void Scoped::makeSign()
    {
        runtime::SignBuilder s;

        s.add("scoped");

        const Scope *scope = _scope;
        while(scope)
        {
            s.add(scope->name());
            scope = scope->scope();
        }

        setSign(s.finish());
    }

}}}}
