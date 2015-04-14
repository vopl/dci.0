#include "alias.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    Alias::Alias()
        : Type(TypeConcrete::alias)
    {
    }

    Alias::~Alias()
    {

    }

    void Alias::setTarget(Type *v)
    {
        _target = v;
    }

}}}}
