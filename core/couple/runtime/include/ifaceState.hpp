#pragma once

#include <cstdint>

namespace dci { namespace couple { namespace runtime
{
    class IfaceStatePtr;

    class IfaceState
    {
        IfaceState(...) = delete;

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

    class IfaceStatePtr
    {
    public:
        IfaceStatePtr();
        IfaceStatePtr(IfaceState *state);
        IfaceStatePtr(const IfaceStatePtr &from);
        IfaceStatePtr(IfaceStatePtr &&from);
        ~IfaceStatePtr();

        IfaceStatePtr operator=(const IfaceStatePtr &from);
        IfaceStatePtr operator=(IfaceStatePtr &&from);

    private:
        IfaceState *_state;

    };
}}}
