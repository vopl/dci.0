#pragma once

#include "api.hpp"
#include <cstdint>

namespace dci { namespace couple { namespace runtime
{
    class APIDCI_COUPLE_RUNTIME IfaceWire
    {
        IfaceWire() = delete;
        void operator=(const IfaceWire&) = delete;

    public:
        using Deleter = void (*)(IfaceWire*wire);
        using UninvolveListener = void (*)(void *userData);

    public:
        IfaceWire(Deleter deleter);
        ~IfaceWire();

        void involve(bool fwd, bool use);
        void listenUninvolve(bool fwd, UninvolveListener listener, void *userData=0);

    private:
        bool    _involvedFwd;
        bool    _involvedBwd;
        UninvolveListener _uninvolvFwdListener;
        void *_uninvolvFwdListenerUserData;
        UninvolveListener _uninvolvBwdListener;
        void *_uninvolvBwdListenerUserData;
        Deleter _deleter;
    };

}}}
