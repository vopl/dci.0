#pragma once

#include "waiter.hpp"
#include <dci/async/waiter.hpp>
#include <dci/mm/intrusiveDlist.hpp>

namespace dci { namespace async { namespace impl
{

    class Waitable
    {
    public:
        Waitable(bool (* canAcquire)(Waitable *), bool (* tryAcquire)(Waitable *));
        ~Waitable();

    public:
        void wait();

        bool canAcquire();
        bool tryAcquire();

        void beginAcquire(WWLink *link);
        void endAcquire(WWLink *link);

    protected:
        dci::mm::IntrusiveDlist<WWLink> _links;

    private:
        bool (* _canAcquire)(Waitable *self);
        bool (* _tryAcquire)(Waitable *self);
    };

}}}
