#pragma once

#include "waitable.hpp"

namespace dci { namespace async { namespace impl
{

    class Semaphore
        : public Waitable
    {
    public:
        Semaphore(std::size_t depth);
        ~Semaphore();

        void lock();
        bool tryLock();
        bool canLock() const;
        void unlock();

    private:
        std::size_t _depth;
    };

}}}
