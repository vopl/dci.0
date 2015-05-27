#pragma once

#include <dci/himpl/faceLayout.hpp>
#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")

#include <dci/async/future.hpp>
#include <dci/couple/runtime/iface.hpp>
#include "error.hpp"
#include <dci/site/testHub.hpp>

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
        static int executeTest(int argc, char *argv[], testHub::Stage stage);

        Manager();
        ~Manager();

        std::error_code run(int argc, char *argv[], testHub::Stage testStage);
        async::Future<std::error_code> stop();

        //outFuture is async::Future<std::error_code, ConcreteIface>
        std::error_code createService(void *outFuture, const couple::runtime::Iid &iid);

        template <class TIface>
        async::Future<std::error_code, TIface> createService();
    };


    template <class TIface>
    async::Future<std::error_code, TIface> Manager::createService()
    {
//        return createService(TIface::_iid).template thenTransform<std::error_code, TIface>([](auto *srcErr, auto *srcValue, auto &dst){
//            if(srcErr) dst.resolveError(std::move(*srcErr));
//            else
//            {
//                assert(0);
//                //dst.resolveValue(TIface(srcValue));
//                dst.resolveValue(TIface());
//            }
//        });

        async::Future<std::error_code, TIface> res{async::FutureNullInitializer()};

        std::error_code ec = createService(&res, TIface::_iid);
        if(ec)
        {
            return async::Future<std::error_code, TIface>(std::move(ec));
        }

        return res;
    }

}}
