#pragma once

#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")

#include <dci/himpl/faceLayout.hpp>
#include "api.hpp"

namespace dci { namespace async
{
    namespace impl
    {
        class Mutex;
    }

    class APIDCI_ASYNC Mutex
        : public himpl::FaceLayout<impl::Mutex>
    {
        Mutex(const Mutex &from) = delete;
        Mutex &operator=(const Mutex &from) = delete;

    public:
        Mutex();
        ~Mutex();

    public:
        void lock();
        bool tryLock();

    public:
        void unlock();

    public:
        bool locked() const;

    public:
        using himpl::FaceLayout<impl::Mutex>::pimpl;
        using himpl::FaceLayout<impl::Mutex>::impl;
    };

}}
