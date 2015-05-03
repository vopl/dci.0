#include <dci/couple/meta/signed.hpp>
#include "impl/signed.hpp"

namespace dci { namespace couple { namespace meta
{
    Signed::Signed()
    {
    }

    Signed::~Signed()
    {
    }

    const runtime::Sign &Signed::sign() const
    {
        return impl().sign();
    }

}}}
