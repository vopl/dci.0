#include "signed.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    Signed::Signed()
    {

    }

    Signed::~Signed()
    {

    }

    const runtime::Sign &Signed::sign() const
    {
        return _sign;
    }

    void Signed::setSign(const runtime::Sign &s)
    {
        _sign = s;
    }

}}}}
