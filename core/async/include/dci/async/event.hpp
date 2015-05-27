#pragma once

#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")

#include <dci/himpl/faceLayout.hpp>
#include "api.hpp"

namespace dci { namespace async
{
    namespace impl
    {
        class Event;
    }

    class APIDCI_ASYNC Event
        : public himpl::FaceLayout<impl::Event>
    {
        Event(const Event &from) = delete;
        Event &operator=(const Event &from) = delete;

    public:
        Event(bool autoReset = true);
        ~Event();

    public:
        void acquire();
        bool tryAcquire();

    public:
        void set();
        void reset();

    public:
        bool isSignalled() const;

    public:
        using himpl::FaceLayout<impl::Event>::pimpl;
        using himpl::FaceLayout<impl::Event>::impl;
    };

}}
