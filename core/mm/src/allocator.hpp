#pragma once

#include <dci/mm/config.hpp>
#include <dci/mm/stack.hpp>

#include "allocator/blocksHolder.hpp"
#include "allocator/bitIndex.hpp"

namespace dci { namespace mm
{

    /////////////////////////////////////////////////////////////////////////////////////////
    class Allocator
    {
    public:
        Allocator() = default;
        ~Allocator() = default;

        void init();
        void deinit();

        ////////////////////////////////////////////////////////////////
        template <std::size_t sizeClass> void *alloc();
        template <std::size_t sizeClass> void free(void *ptr);
        template <std::size_t sizeClass> std::size_t occupied() const;

        void *alloc(std::size_t size);
        void free(void *ptr);
        std::size_t size(void *ptr);
        std::size_t occupied() const;

        ////////////////////////////////////////////////////////////////
        void *allocateBlock();
        void deallocateBlock(void *ptr);

        ////////////////////////////////////////////////////////////////
        const dci::mm::Stack *stackAlloc();
        void stackFree(const dci::mm::Stack *stack);
        void stackCompact(const dci::mm::Stack *stack);

        ////////////////////////////////////////////////////////////////
        bool vmAccessHandler(void *addr);

    private:
        using BlocksBitIndex = mm::allocator::BitIndex<ConfigHeap::_blocksAmount>;
        using StacksBitIndex = mm::allocator::BitIndex<ConfigStack::_stacksAmount>;

        static constexpr std::size_t _blocksBitIndexAlignedSize = ConfigHeap::alignUp(sizeof(BlocksBitIndex), ConfigMemory::_pageSize);
        static constexpr std::size_t _blocksPad = ConfigHeap::_blockSize;
        static constexpr std::size_t _blocksAlignedSize = ConfigHeap::_blocksAmount * ConfigHeap::_blockSize;

        static constexpr std::size_t _stacksBitIndexAlignedSize = ConfigHeap::alignUp(sizeof(StacksBitIndex), ConfigMemory::_pageSize);
        static constexpr std::size_t _stacksPad = ConfigStack::_stackPages*ConfigMemory::_pageSize;
        static constexpr std::size_t _stacksAlignedSize = ConfigStack::_stacksAmount * ConfigStack::_stackPages*ConfigMemory::_pageSize;

        static constexpr std::size_t _vmSize =
                _blocksBitIndexAlignedSize + _blocksPad + _blocksAlignedSize +
                _stacksBitIndexAlignedSize + _stacksPad + _stacksAlignedSize;


        void *_vm {nullptr};

        BlocksBitIndex *_blocksBitIndex {nullptr};
        void *_blocks {nullptr};

        StacksBitIndex *_stacksBitIndex {nullptr};
        void *_stacks {nullptr};

    private:
        allocator::BlocksHolder _blocksHolders[ConfigHeap::_maxSizeClassIndex+1];
    };

    extern Allocator g_allocator;
}}
