#pragma once

#include <dci/himpl/faceLayout.hpp>
#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")

#include <dci/async/future.hpp>
#include <dci/couple/runtime/iface.hpp>
#include <system_error>

namespace dci { namespace site
{
    namespace impl
    {
        class Manager;
    }

    class Manager
        : public himpl::FaceLayout<impl::Manager>
    {
        Manager() = delete;
        ~Manager() = delete;
        void operator=(const Manager &) = delete;

    public:
        async::Future<std::error_code, couple::runtime::Iface> getServiceInstance(const couple::runtime::Iid &iid);

        template <class TIface>
        async::Future<std::error_code, TIface> getServiceInstance();
    };


    template <class TIface>
    async::Future<std::error_code, TIface> Manager::getServiceInstance()
    {
        return getServiceInstance(TIface::_iid).template thenTransform<std::error_code, TIface>([](auto *srcErr, auto *srcValue, auto &dst){
            if(srcErr) dst.resolveError(std::move(*srcErr));
            else
            {
                assert(0);
                //dst.resolveValue(TIface(srcValue));
                dst.resolveValue(TIface());
            }
        });
    }

}}
