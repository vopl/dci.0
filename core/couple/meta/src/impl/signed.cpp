#include "signed.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    Signed::Signed()
    {

    }

    Signed::~Signed()
    {

    }

    const Sign &Signed::sign() const
    {
        return _sign;
    }

    void Signed::setSign(const Sign &s)
    {
        _sign = s;
    }

}}}}
