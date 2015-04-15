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
        s.add(scopesSign());
        s.add(_name);

        s.add(Compound<Struct>::elements().size());
        for(auto v : Compound<Struct>::elements())
        {
            s.add(v->sign());
        }

        s.add(Compound<Variant>::elements().size());
        for(auto v : Compound<Variant>::elements())
        {
            s.add(v->sign());
        }

        s.add(Compound<Enum>::elements().size());
        for(auto v : Compound<Enum>::elements())
        {
            s.add(v->sign());
        }

        s.add(Compound<Alias>::elements().size());
        for(auto v : Compound<Alias>::elements())
        {
            s.add(v->sign());
        }

        s.add(Compound<Iface>::elements().size());
        for(auto v : Compound<Iface>::elements())
        {
            s.add(v->sign());
        }

        s.add(Compound<Scope>::elements().size());
        for(auto v : Compound<Scope>::elements())
        {
            s.add(v->sign());
        }

        setSign(s.finish());
    }

}}}}
