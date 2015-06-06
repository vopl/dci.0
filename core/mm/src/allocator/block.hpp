#pragma once

#include <dci/mm/config.hpp>

#include <dci/mm/intrusiveDlist.hpp>

#include <cstdint>
#include <type_traits>

namespace dci { namespace mm { namespace allocator
{
    ////////////////////////////////////////////////////////////////////////////////
    class BlocksHolder;

    ////////////////////////////////////////////////////////////////////////////////
    class Block
    {
    public:
        static Block *allocateBlock(BlocksHolder *thisBlockHolder, std::size_t sizeClass);
        static void deallocateBlock(Block *block);

    public:
        Block(BlocksHolder *thisBlockHolder, std::size_t sizeClass);
        ~Block();


        template <std::size_t sizeClass> void *alloc();
        template <std::size_t sizeClass> void free(void *ptr);

        void *alloc();
        void free(void *ptr);

        bool empty() const;
        bool full() const;

        std::size_t getSizeClass() const;

    private:
        void *allocWithoutCounters();
        void freeWithoutCounters(void *ptr);

    private:
         using Counter = std::uint64_t;
         struct ElementHeader
         {
             ElementHeader *_next;
         };

    private:
        mm::IntrusiveDlistElement<Block> _intrusiveDequeElement;

    private:
        ElementHeader *_nextFreeElement;

    private:
        BlocksHolder *_thisBlockHolder;
        Counter _allocated;
        std::size_t _sizeClass;
        std::size_t _elementsAmount;

        alignas(ConfigMemory::_cacheLineSize) char _elementsArea[ConfigHeap::_blockSize - ConfigMemory::_cacheLineSize];
    };

    ////////////////////////////////////////////////////////////////////////////////
    inline IntrusiveDlistElement<Block> *intrusiveDlistElementCast(Block *e)
    {
        //static_assert(0 == offsetof(Block, _intrusiveDequeElement), "");
        return (IntrusiveDlistElement<Block> *)(void *)e;
    }

    ////////////////////////////////////////////////////////////////////////////////
    inline Block *intrusiveDlistElementCast(IntrusiveDlistElement<Block> *e, Block *)
    {
        return (Block *)(void *)e;
    }

    ////////////////////////////////////////////////////////////////////////////////
    static_assert(ConfigHeap::_blockSize == sizeof(Block), "Block must be ConfigHeap::_blockSize bytes long");
    static_assert(std::is_standard_layout<Block>::value, "Block must satisfy StandardLayoutType");

}}}
