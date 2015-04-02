#pragma once

#include "ifaceId.hpp"
#include "ifaceState.hpp"

namespace dci { namespace couple { namespace runtime
{
    class Iface
    {
        Iface() = delete;

    protected:
        Iface(IfaceState *state, bool fwd);
        Iface(Iface &&from);

    public:
        ~Iface();

        Iface &operator=(Iface &&from);

        IfaceState *state();

    private:
        IfaceState *_state;
        bool _fwd;
    };

}}}
