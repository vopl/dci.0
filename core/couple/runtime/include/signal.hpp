#pragma once

namespace dci { namespace couple { namespace runtime
{
    template <class F>
    class Signal
    {

    public:
        template <class F2>
        int connect(const F2 &)
        {
            static int k = 220;
        }
    };
}}}
