#pragma once

#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")

#include <dci/himpl/faceLayout.hpp>
#include "api.hpp"

namespace dci { namespace async
{
    namespace impl
    {
        class Waitable;
    }

    class APIDCI_ASYNC Waitable
        : public himpl::FaceLayout<impl::Waitable>
    {
    public:
        using himpl::FaceLayout<impl::Waitable>::FaceLayout;
        Waitable();
        ~Waitable();

        void wait();
    };

}}
