#pragma once

#include "ifaceId.hpp"
#include "ifaceState.hpp"

namespace dci { namespace couple { namespace runtime
{
    class Iface
    {
        Iface() = delete;
    public:
        Iface(IfaceState *state);
        Iface(const Iface &from);
        Iface(Iface &&from);
        ~Iface();

        Iface &operator=(const Iface &from);
        Iface &operator=(Iface &&from);

        IfaceState *state();

    private:
        IfaceState *_state;
    };

}}}
