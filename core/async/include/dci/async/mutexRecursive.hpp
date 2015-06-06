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
        class MutexRecursive;
    }

    class APIDCI_ASYNC MutexRecursive
        : public himpl::FaceLayout<impl::MutexRecursive, Waitable, Lockable>
    {
        MutexRecursive(const MutexRecursive &from) = delete;
        MutexRecursive &operator=(const MutexRecursive &from) = delete;

    public:
        MutexRecursive();
        ~MutexRecursive();

    public:
        void lock();
        bool tryLock();
        bool canLock() const;
        void unlock();
    };

}}
