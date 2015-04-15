#include "scoped.hpp"
#include "scope.hpp"
#include "signBuilder.hpp"

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

    Sign Scoped::scopesSign() const
    {
        SignBuilder s;

        const Scope *scope = _scope;
        while(scope)
        {
            s.add(scope->sign());
            scope = scope->scope();
        }

        return s.finish();
    }

}}}}
