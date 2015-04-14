#include "method.hpp"

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

}}}}
