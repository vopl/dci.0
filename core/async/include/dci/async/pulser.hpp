#pragma once

#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")

#include <dci/himpl/faceLayout.hpp>
#include "api.hpp"

namespace dci { namespace async
{
    namespace impl
    {
        class Pulser;
    }

    class APIDCI_ASYNC Pulser
        : public himpl::FaceLayout<impl::Pulser>
    {
        Pulser(const Pulser &from) = delete;
        Pulser &operator=(const Pulser &from) = delete;

    public:
        Pulser();
        ~Pulser();

    public:
        void wait();
        void pulse();

    public:
        using himpl::FaceLayout<impl::Pulser>::pimpl;
        using himpl::FaceLayout<impl::Pulser>::impl;
    };

}}
