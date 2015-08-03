#pragma once

#include "api.hpp"
#include "interfaceId.hpp"
#include "interfaceWire.hpp"

namespace dci { namespace couple { namespace runtime
{
    class APIDCI_COUPLE_RUNTIME Interface
    {
        Interface() = delete;

    public:
        Interface(InterfaceWire *wire, bool fwd);
        Interface(Interface &&from);

        ~Interface();

        Interface &operator=(Interface &&from);
        void assign(InterfaceWire *wire, bool fwd);

        InterfaceWire *wire();
        bool oppositeInvolved() const;

    private:
        InterfaceWire*_wire;
        bool _fwd;
    };

}}}
