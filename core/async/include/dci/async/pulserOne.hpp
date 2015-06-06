#pragma once

#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")

#include <dci/himpl/faceLayout.hpp>
#include "waitable.hpp"
#include "api.hpp"

namespace dci { namespace async
{
    namespace impl
    {
        class PulserOne;
    }

    class APIDCI_ASYNC PulserOne
        : public himpl::FaceLayout<impl::PulserOne, Waitable>
    {
        PulserOne(const PulserOne &from) = delete;
        PulserOne &operator=(const PulserOne &from) = delete;

    public:
        PulserOne();
        ~PulserOne();

    public:
        void acquire();
        void pulse();
    };

}}
