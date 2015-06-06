#pragma once

#include "waitable.hpp"

namespace dci { namespace async { namespace impl
{
    class PulserAll
        : public Waitable
    {
    public:
        PulserAll();
        ~PulserAll();

        void acquire();
        bool tryAcquire();
        bool canAcquire() const;
        void pulse();

    private:
        bool _ready;
    };

}}}
