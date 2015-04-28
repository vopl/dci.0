#include "alias.hpp"
#include <dci/couple/runtime/signBuilder.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    Alias::Alias()
        : Type(TypeConcrete::alias)
    {
    }

    Alias::~Alias()
    {

    }

    void Alias::setType(Type *v)
    {
        _type = v;
    }

    const Type *Alias::type() const
    {
        return _type;
    }

    void Alias::makeSign()
    {
        runtime::SignBuilder s;

        s.add("alias");
        s.add(sign());
        s.add(_type->concreteSign());
        s.add(name());

        setSign(s.finish());
    }

}}}}
