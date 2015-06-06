#pragma once

#include "waitable.hpp"
#include "lockable.hpp"

namespace dci { namespace async { namespace impl
{

    class Mutex
        : public Waitable
        , public Lockable
    {
    public:
        Mutex();
        ~Mutex();

    public:
        void lock();
        bool tryLock();
        bool canLock() const;
        void unlock();

    private:
        bool _locked;
    };

}}}
