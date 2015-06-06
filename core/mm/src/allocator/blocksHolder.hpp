#pragma once

#include "block.hpp"
#include <dci/mm/intrusiveDlist.hpp>

namespace dci { namespace mm { namespace allocator
{
    ////////////////////////////////////////////////////////////////////////////////
    class BlocksHolder
    {
    public:
        BlocksHolder() = default;
        ~BlocksHolder();

    public:
        template <std::size_t sizeClass> void *alloc();

        void *alloc(std::size_t sizeClass);

    public:
        //callbacks to block
        void blockEmptyToMiddle(Block *block);
        void blockMiddleToEmpty(Block *block);
        void blockMiddleToFull(Block *block);
        void blockFullToMiddle(Block *block);

    private:
        IntrusiveDlist<Block> _empty;
        IntrusiveDlist<Block> _middle;
        IntrusiveDlist<Block> _full;
    };
}}}
