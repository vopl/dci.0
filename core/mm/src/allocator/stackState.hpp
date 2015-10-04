#pragma once

#include <dci/mm/config.hpp>
#include <dci/mm/stack.hpp>

namespace dci { namespace mm { namespace allocator
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    struct StackState
        : public dci::mm::Stack
    {
        unsigned _valgringId;
    };

}}}
