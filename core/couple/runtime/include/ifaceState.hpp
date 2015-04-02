#pragma once

#include <cstdint>

namespace dci { namespace couple { namespace runtime
{
    class IfaceState
    {
        IfaceState() = delete;
        void operator=(const IfaceState &) = delete;

    public:
        using Deleter = void (*)(IfaceState *state);

    public:
        IfaceState(Deleter deleter);
        ~IfaceState();

        void involve(bool fwd, bool use);

    private:
        bool    _involvedFwd;
        bool    _involvedBwd;
        Deleter _deleter;
    };

}}}
