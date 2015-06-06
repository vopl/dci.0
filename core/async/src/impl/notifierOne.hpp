#pragma once

#include "waitable.hpp"

namespace dci { namespace async { namespace impl
{
    class NotifierOne
        : public Waitable
    {
    public:
        NotifierOne();
        ~NotifierOne();

        void acquire();
        bool tryAcquire();
        bool canAcquire() const;

        void notify();

    private:
        bool _ready;
    };

}}}
