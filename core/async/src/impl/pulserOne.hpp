#pragma once

#include "waitable.hpp"

namespace dci { namespace async { namespace impl
{
    class PulserOne
        : public Waitable
    {
    public:
        PulserOne();
        ~PulserOne();

        void acquire();
        bool tryAcquire();
        bool canAcquire() const;
        void pulse();

    private:
        bool _ready;
    };

}}}
