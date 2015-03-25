#pragma once

#include <cstdint>

namespace dci { namespace couple { namespace runtime
{
    class IfaceStatePtr;

    class IfaceState
    {
        IfaceState() = delete;
        void operator=(const IfaceState &) = delete;

    public:
        using Deleter = void (*)(IfaceState *state);

    public:
        IfaceState(Deleter deleter);
        ~IfaceState();

        void incRef();
        void decRef();

    private:
        std::size_t _refs;
        Deleter     _deleter;
    };

}}}
