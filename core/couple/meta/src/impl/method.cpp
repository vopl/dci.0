#include "method.hpp"
#include <dci/couple/runtime/signBuilder.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    Method::Method()
    {

    }

    Method::~Method()
    {

    }

    void Method::setDirection(CallDirection v)
    {
        _direction = v;
    }

    CallDirection Method::direction() const
    {
        return _direction;
    }

    void Method::setNoreply(bool v)
    {
        _noreply = v;
    }

    bool Method::noreply() const
    {
        return _noreply;
    }

    void Method::makeSign()
    {
        runtime::SignBuilder s;

        s.add("method");
        s.add(static_cast<std::uint32_t>(_direction));
        s.add(_name);

        for(auto v : Compound<Attribute>::elements())
        {
            s.add(v->sign());
        }

        for(auto v : Compound<Type>::elements())
        {
            s.add(v->concreteSign());
        }

        setSign(s.finish());
    }

}}}}
