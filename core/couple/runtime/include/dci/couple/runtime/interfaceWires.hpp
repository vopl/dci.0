#pragma once

#include "api.hpp"
#include "interfaceId.hpp"
#include "interfaceWire.hpp"
#include <cstdint>

namespace dci { namespace couple { namespace runtime
{
    enum class InterfaceDirection
    {
        fwd,
        bwd,
    };

    struct InterfaceWithWiresInitializer {};

    class APIDCI_COUPLE_RUNTIME InterfaceWires
    {
        InterfaceWires(const InterfaceWires&) = delete;
        void operator=(const InterfaceWires&) = delete;

    public:
        using UninvolveListener = void (*)(void *userData);

    public:
        InterfaceWires();
        virtual ~InterfaceWires();

        void involve(InterfaceDirection direction, bool use);
        bool involved(InterfaceDirection direction);
        void listenUninvolve(InterfaceDirection direction, UninvolveListener listener, void *userData=0);

    public:
        virtual InterfaceWire* concrete(const Iid &iid) = 0;

    private:
        bool    _involvedFwd;
        bool    _involvedBwd;
        UninvolveListener _uninvolvFwdListener;
        void *_uninvolvFwdListenerUserData;
        UninvolveListener _uninvolvBwdListener;
        void *_uninvolvBwdListenerUserData;
    };

}}}
