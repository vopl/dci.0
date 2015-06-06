#pragma once

#include "waitable.hpp"

namespace dci { namespace async { namespace impl
{
    class Event
        : public Waitable
    {
    public:
        Event();
        ~Event();

        void acquire();
        bool tryAcquire();
        bool canAcquire() const;

        void set();
        void reset();

    private:
        bool _ready;
    };

}}}
