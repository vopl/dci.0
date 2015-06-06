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
        class Barrier;
    }

    class APIDCI_ASYNC Barrier
        : public himpl::FaceLayout<impl::Barrier, Waitable>
    {
        Barrier(const Barrier &from) = delete;
        Barrier &operator=(const Barrier &from) = delete;

    public:
        Barrier(std::size_t depth);
        ~Barrier();

    public:
        void traverse();
        bool tryTraverse();
        bool canTraverse() const;
    };

}}
