#include <dci/site/manager.hpp>
#include "impl/manager.hpp"

#include <dci/async/functions.hpp>

namespace dci { namespace site
{
    std::error_code Manager::createService(void *outFuture, const couple::runtime::Iid &iid)
    {
        return impl().createService(outFuture, iid);
    }

}}
