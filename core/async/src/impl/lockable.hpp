#pragma once

namespace dci { namespace async { namespace impl
{

    class Lockable
    {
    public:
        Lockable();
        ~Lockable();

        void lock();
        void unlock();
    };

}}}
