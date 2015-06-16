#pragma once

#include <dci/himpl/faceLayout.hpp>
#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")

#include <dci/async/future.hpp>
#include <dci/couple/runtime/interface.hpp>
#include "error.hpp"
#include <dci/site/test.hpp>

namespace dci { namespace site
{
    namespace impl
    {
        class Manager;
    }

    class APIDCI_SITE Manager
        : public himpl::FaceLayout<impl::Manager>
    {
        Manager(const Manager &) = delete;
        void operator=(const Manager &) = delete;

    public:
        static std::string generateManifest(const std::string &mainBinaryFullPath);
        static int executeTest(int argc, char *argv[], TestStage stage);

        Manager();
        ~Manager();

        std::error_code run(int argc, char *argv[], TestStage testStage);
        async::Future<std::error_code> stop();

        //outFuture is async::Future<std::error_code, ConcreteInterface>
        std::error_code createService(void *outFuture, const couple::runtime::Iid &iid);

        template <class TInterface>
        async::Future<std::error_code, TInterface> createService();
    };


    template <class TInterface>
    async::Future<std::error_code, TInterface> Manager::createService()
    {
//        return createService(TInterface::_iid).template thenTransform<std::error_code, TInterface>([](auto *srcErr, auto *srcValue, auto &dst){
//            if(srcErr) dst.resolveError(std::move(*srcErr));
//            else
//            {
//                assert(0);
//                //dst.resolveValue(TInterface(srcValue));
//                dst.resolveValue(TInterface());
//            }
//        });

        async::Future<std::error_code, TInterface> res{async::FutureNullInitializer()};

        std::error_code ec = createService(&res, TInterface::_iid);
        if(ec)
        {
            return async::Future<std::error_code, TInterface>(std::move(ec));
        }

        return res;
    }

}}
