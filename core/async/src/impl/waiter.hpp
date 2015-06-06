#pragma once

#include <dci/async/waiter.hpp>
#include "ctx/coro.hpp"
#include <cstdint>

namespace dci { namespace async { namespace impl
{

    class Waitable;

    class Waiter
    {
    public:
        Waiter(WWLink *links, std::size_t amount);
        ~Waiter();

        std::size_t any();
        void all();
        void each();

    public:
        bool readyOffer(WWLink *link);

    private:
        WWLink *    _links;
        std::size_t _amount;

        enum class Mode
        {
            null,
            any,
            all,
            each
        } _mode;

        union
        {
            std::size_t _acquiredAmount;
            std::size_t _acquiredIndex;
        } _perModeState;

    private:
        ctx::Coro * _coro;
    };

}}}
