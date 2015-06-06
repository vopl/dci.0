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
        class Event;
    }

    class APIDCI_ASYNC Event
        : public himpl::FaceLayout<impl::Event, Waitable>
    {
        Event(const Event &from) = delete;
        Event &operator=(const Event &from) = delete;

    public:
        Event();
        ~Event();

    public:
        void acquire();
        bool tryAcquire();
        bool canAcquire() const;

        void set();
        void reset();
    };

}}
