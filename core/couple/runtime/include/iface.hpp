#pragma once

#include "ifaceId.hpp"
#include "ifaceWire.hpp"

namespace dci { namespace couple { namespace runtime
{
    class Iface
    {
        Iface() = delete;

    protected:
        Iface(IfaceWire *wire, bool fwd);
        Iface(Iface &&from);

    public:
        ~Iface();

        Iface &operator=(Iface &&from);

        IfaceWire *wire();

    private:
        IfaceWire*_wire;
        bool _fwd;
    };

}}}
