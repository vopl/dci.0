#pragma once

#include "waitable.hpp"

namespace dci { namespace async { namespace impl
{

    class Barrier
        : public Waitable
    {
    public:
        Barrier(std::size_t depth);
        ~Barrier();

        void traverse();
        bool tryTraverse();
        bool canTraverse() const;

    private:
        std::size_t _depth;
        std::size_t _amount;
    };

}}}
