#include <dci/site/manager.hpp>
#include "impl/manager.hpp"

#include <dci/async/functions.hpp>

namespace dci { namespace site
{
    std::string Manager::generateManifest(const std::string &mainBinaryFullPath)
    {
        return impl::Manager::generateManifest(mainBinaryFullPath);
    }

    int Manager::executeTest(int argc, char *argv[], testHub::Stage stage)
    {
        return impl::Manager::executeTest(argc, argv, stage, nullptr);
    }

    Manager::Manager()
        : himpl::FaceLayout<impl::Manager>()
    {
    }

    Manager::~Manager()
    {
    }

    std::error_code Manager::run(int argc, char *argv[], testHub::Stage testStage)
    {
        return impl().run(argc, argv, testStage);
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
