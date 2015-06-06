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
        class Semaphore;
    }

    class APIDCI_ASYNC Semaphore
        : public himpl::FaceLayout<impl::Semaphore, Waitable, Lockable>
    {
        Semaphore(const Semaphore &from) = delete;
        Semaphore &operator=(const Semaphore &from) = delete;

    public:
        Semaphore(std::size_t depth);
        ~Semaphore();

    public:
        void lock();
        bool tryLock();
        bool canLock() const;
        void unlock();
    };

}}
