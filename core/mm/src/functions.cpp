#include <dci/mm/functions.hpp>

#include "system.hpp"
#include "allocator.hpp"

#include "functions.ipp"
#include "allocator.ipp"
#include "allocator/blocksHolder.ipp"
#include "allocator/block.ipp"
#include "utils/sized_cast.ipp"
#include "allocator/bitIndex.ipp"
#include "allocator/bitIndex/level.ipp"

#include <cstdlib>

namespace dci { namespace mm
{
    const Stack *stackAlloc()
    {
        return g_allocator.stackAlloc();
    }

    void stackFree(const Stack *stack)
    {
        return g_allocator.stackFree(stack);
    }

    void stackCompact(const Stack *stack)
    {
        return g_allocator.stackCompact(stack);
    }

    namespace
    {
        /////////////////////////////////////////////////////////////////////////////////
        template <std::size_t sizeClass>
        std::uintptr_t bySizeClassInstantiate()
        {
            return
                bySizeClassInstantiate<ConfigHeap::evalPrevSizeClass(sizeClass-1)>() +
                reinterpret_cast<std::uintptr_t>(&details::allocBySizeClass<sizeClass>) +
                reinterpret_cast<std::uintptr_t>(&details::freeBySizeClass<sizeClass>) +
                reinterpret_cast<std::uintptr_t>(&details::occupiedBySizeClass<sizeClass>) +
                0;
        }

        /////////////////////////////////////////////////////////////////////////////////
        template <>
        std::uintptr_t bySizeClassInstantiate<ConfigHeap::_minSize>()
        {
            return
                reinterpret_cast<std::uintptr_t>(&details::allocBySizeClass<ConfigHeap::_minSize>) +
                reinterpret_cast<std::uintptr_t>(&details::freeBySizeClass<ConfigHeap::_minSize>) +
                reinterpret_cast<std::uintptr_t>(&details::occupiedBySizeClass<ConfigHeap::_minSize>) +
                0;
        }

        static const volatile std::uintptr_t bySizeClassInstantiateSideEffect = bySizeClassInstantiate<ConfigHeap::_maxSize>();
    }

}}
