#pragma once

#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")

#include <dci/himpl/faceLayout.hpp>
#include "waitable.hpp"
#include "lockable.hpp"
#include "api.hpp"

namespace dci { namespace async
{
    namespace impl
    {
        class Mutex;
    }

    class APIDCI_ASYNC Mutex
        : public himpl::FaceLayout<impl::Mutex, Waitable, Lockable>
    {
        Mutex(const Mutex &from) = delete;
        Mutex &operator=(const Mutex &from) = delete;

    public:
        Mutex();
        ~Mutex();

    public:
        void lock();
        bool tryLock();
        bool canLock() const;
        void unlock();
    };

}}
