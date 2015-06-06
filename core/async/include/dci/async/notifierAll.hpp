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
        class NotifierAll;
    }

    class APIDCI_ASYNC NotifierAll
        : public himpl::FaceLayout<impl::NotifierAll, Waitable>
    {
        NotifierAll(const NotifierAll &from) = delete;
        NotifierAll &operator=(const NotifierAll &from) = delete;

    public:
        NotifierAll();
        ~NotifierAll();

    public:
        void acquire();
        bool tryAcquire();
        bool canAcquire() const;

        void notify();
    };

}}
