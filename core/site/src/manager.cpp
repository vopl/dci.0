#include <dci/site/manager.hpp>
#include "impl/manager.hpp"

#include <dci/async/functions.hpp>

namespace dci { namespace site
{
    std::string Manager::generateManifest(const std::string &mainBinaryFullPath)
    {
        return impl::Manager::generateManifest(mainBinaryFullPath);
    }

    Manager::Manager()
        : himpl::FaceLayout<impl::Manager>()
    {
    }

    Manager::~Manager()
    {
    }

    std::error_code Manager::run()
    {
        return impl().run();
    }

    async::Future<std::error_code> Manager::stop()
    {
        return impl().stop();
    }

    std::error_code Manager::createService(void *outFuture, const couple::runtime::Iid &iid)
    {
        return impl().createService(outFuture, iid);
    }

}}
