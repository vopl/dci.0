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

    class APIDCI_ASYNC PulserAll
        : public himpl::FaceLayout<impl::PulserAll, Waitable>
    {
        PulserAll(const PulserAll &from) = delete;
        PulserAll &operator=(const PulserAll &from) = delete;

    public:
        PulserAll();
        ~PulserAll();

    public:
        void acquire();
        void pulse();
    };

}}
