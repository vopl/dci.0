#pragma once

#include "api.hpp"
#include "interfaceId.hpp"
#include "interfaceWire.hpp"
#include "interfaceWires.hpp"

namespace dci { namespace couple { namespace runtime
{
    class APIDCI_COUPLE_RUNTIME Interface
    {
        Interface() = delete;

    public:
        Interface(InterfaceWires *wire, bool fwd);
        Interface(Interface &&from);

        ~Interface();

        Interface &operator=(Interface &&from);
        void assign(InterfaceWires *wire, bool fwd);

        InterfaceWires *wires();
        bool fwd() const;
        bool oppositeInvolved() const;

        operator bool() const;
        bool operator !() const;

    protected:
        struct NullInterfaceInitializer {};
        Interface(NullInterfaceInitializer);

    private:
        InterfaceWires*_wires;
        bool _fwd;
    };

}}}
