#include "scoped.hpp"

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

}}}}
