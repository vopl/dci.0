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

    void Method::setNowait(bool v)
    {
        _nowait = v;
    }

    void Method::setResultType(Type *v)
    {
        _resultType = v;
    }

    CallDirection Method::direction() const
    {
        return _direction;
    }

    bool Method::nowait() const
    {
        return _nowait;
    }

    const Type *Method::resultType() const
    {
        return _resultType;
    }

    void Method::makeSign()
    {
        runtime::SignBuilder s;

        s.add("method");
        s.add((std::uint32_t)_direction);
        s.add((std::uint32_t)_nowait);
        s.add(_resultType->concreteSign());
        s.add(_name);

        for(auto v : _elements)
        {
            s.add(v->sign());
        }

        setSign(s.finish());
    }

}}}}
