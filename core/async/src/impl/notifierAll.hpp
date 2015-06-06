#pragma once

#include "waitable.hpp"

namespace dci { namespace async { namespace impl
{
    class NotifierAll
        : public Waitable
    {
    public:
        NotifierAll();
        ~NotifierAll();

        void acquire();
        bool tryAcquire();
        bool canAcquire() const;

        void notify();

    private:
        bool _ready;
    };

}}}
