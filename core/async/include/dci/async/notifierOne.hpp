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
        class NotifierOne;
    }

    class APIDCI_ASYNC NotifierOne
        : public himpl::FaceLayout<impl::NotifierOne, Waitable>
    {
        NotifierOne(const NotifierOne &from) = delete;
        NotifierOne &operator=(const NotifierOne &from) = delete;

    public:
        NotifierOne();
        ~NotifierOne();

    public:
        void acquire();
        bool tryAcquire();
        bool canAcquire() const;

        void notify();
    };

}}
