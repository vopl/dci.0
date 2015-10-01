#pragma once

#include "api.hpp"
#include "interfaceId.hpp"
#include "interfaceWire.hpp"
#include "interfaceWires.hpp"

namespace dci { namespace couple { namespace runtime
{
    class APIDCI_COUPLE_RUNTIME Interface;
    using InterfaceOpposite = Interface;

    class APIDCI_COUPLE_RUNTIME Interface
    {
        Interface() = delete;
        Interface(const Interface &) = delete;
        void operator=(const Interface &) = delete;

    public:
        Interface(InterfaceDirection direction);
        Interface(InterfaceDirection direction, InterfaceWires *wires);
        Interface(Interface &&from);
        Interface(InterfaceOpposite &from);

        virtual ~Interface();

        Interface &operator=(Interface &&from);
        Interface &operator=(InterfaceOpposite &from);

        void assignWires(InterfaceWires *wires);
        virtual void virtualAssignOpposite(InterfaceOpposite &from);

        InterfaceDirection direction() const;
        InterfaceWires *wires();

        operator bool() const;
        bool operator !() const;

    private:
        InterfaceDirection  _direction;
        InterfaceWires*     _wires;
    };

}}}
