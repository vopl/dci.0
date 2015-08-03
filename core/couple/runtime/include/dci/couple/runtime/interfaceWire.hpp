#pragma once

#include "api.hpp"
#include <cstdint>

namespace dci { namespace couple { namespace runtime
{
    class APIDCI_COUPLE_RUNTIME InterfaceWire
    {
        InterfaceWire() = delete;
        void operator=(const InterfaceWire&) = delete;

    public:
        using Deleter = void (*)(InterfaceWire*wire);
        using UninvolveListener = void (*)(void *userData);

    public:
        InterfaceWire(Deleter deleter);
        ~InterfaceWire();

        void involve(bool fwd, bool use);
        bool involved(bool fwd);
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
