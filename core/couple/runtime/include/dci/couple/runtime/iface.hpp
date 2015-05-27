#pragma once

#include "api.hpp"
#include "ifaceId.hpp"
#include "ifaceWire.hpp"

namespace dci { namespace couple { namespace runtime
{
    class APIDCI_COUPLE_RUNTIME Iface
    {
        Iface() = delete;

    public:
        Iface(IfaceWire *wire, bool fwd);
        Iface(Iface &&from);

        ~Iface();

        Iface &operator=(Iface &&from);
        void assign(IfaceWire *wire, bool fwd);

        IfaceWire *wire();

    private:
        IfaceWire*_wire;
        bool _fwd;
    };

}}}
