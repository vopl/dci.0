#include <instance.hpp>
#include <dci/async/functions.hpp>

namespace dci { namespace site
{
    async::Future<std::error_code, couple::runtime::Iface> Instance::getServiceInstance(const couple::runtime::Iid &iid)
    {
        assert(0);

        //return async::Future<std::error_code, couple::runtime::Iface>(std::error_code());

        return async::spawn([] (async::Promise<std::error_code, couple::runtime::Iface> p) {
            p.resolveError(std::error_code());
        });
    }
}}
