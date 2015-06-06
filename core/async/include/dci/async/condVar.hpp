#pragma once

#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")

#include <dci/himpl/faceLayout.hpp>
#include "waitable.hpp"
#include "lockable.hpp"
#include "api.hpp"

namespace dci { namespace async
{
    namespace impl
    {
        class CondVar;
    }

    class APIDCI_ASYNC CondVar
        : public himpl::FaceLayout<impl::CondVar, Waitable>
    {
        CondVar(const CondVar &from) = delete;
        CondVar &operator=(const CondVar &from) = delete;

    public:
        CondVar();
        ~CondVar();

        void acquire(Lockable &);
        bool tryAcquire(Lockable &);
        bool canAcquire() const;

        void notifyOne();
        void notifyAll();

    };

}}
