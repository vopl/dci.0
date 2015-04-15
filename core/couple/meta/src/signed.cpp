#include <signed.hpp>
#include "impl/signed.hpp"
#include "impl/sign.hpp"

namespace dci { namespace couple { namespace meta
{
    const Sign &Signed::sign() const
    {
        return himpl::impl2Face<Sign>(impl().sign());
    }

}}}
