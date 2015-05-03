#pragma once

#include <cstdint>

namespace dci { namespace couple { namespace runtime
{
    class IfaceWire
    {
        IfaceWire() = delete;
        void operator=(const IfaceWire&) = delete;

    public:
        using Deleter = void (*)(IfaceWire*state);

    public:
        IfaceWire(Deleter deleter);
        ~IfaceWire();

        void involve(bool fwd, bool use);

    private:
        bool    _involvedFwd;
        bool    _involvedBwd;
        Deleter _deleter;
    };

}}}
