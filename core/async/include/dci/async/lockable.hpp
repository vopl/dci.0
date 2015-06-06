#pragma once

#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")

#include <dci/himpl/faceLayout.hpp>
#include "api.hpp"

namespace dci { namespace async
{
    namespace impl
    {
        class Lockable;
    }

    class APIDCI_ASYNC Lockable
        : public himpl::FaceLayout<impl::Lockable>
    {
    public:
        using himpl::FaceLayout<impl::Lockable>::FaceLayout;
        Lockable();
        ~Lockable();

        void lock();
        void unlock();
    };

}}
