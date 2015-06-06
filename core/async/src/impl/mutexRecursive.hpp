#pragma once

#include "waitable.hpp"
#include "lockable.hpp"
#include "ctx/coro.hpp"

namespace dci { namespace async { namespace impl
{

    class MutexRecursive
        : public Waitable
        , public Lockable
    {
    public:
        MutexRecursive();
        ~MutexRecursive();

    public:
        void lock();
        bool tryLock();
        bool canLock() const;
        void unlock();

    private:
        ctx::Coro * _owner;
        std::size_t _counter;
    };

}}}
