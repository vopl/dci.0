#pragma once

#include "api.hpp"
#include "config.hpp"
#include "stack.hpp"

namespace dci { namespace mm
{
    namespace details
    {
        ////////////////////////////////////////////////////////////////
        template <std::size_t sizeClass> APIDCI_MM void *allocBySizeClass(std::size_t size);
        template <std::size_t sizeClass> APIDCI_MM void freeBySizeClass(void *ptr);
        template <std::size_t sizeClass> APIDCI_MM std::size_t occupiedBySizeClass();
    }

    ////////////////////////////////////////////////////////////////
    template <std::size_t size> void *alloc()
    {
        return details::allocBySizeClass<ConfigHeap::evalSizeClass(size)>(size);
    }

    template <std::size_t size> void free(void *ptr)
    {
        return details::freeBySizeClass<ConfigHeap::evalSizeClass(size)>(ptr);
    }

    template <std::size_t size> std::size_t occupied()
    {
        return details::occupiedBySizeClass<ConfigHeap::evalSizeClass(size)>();
    }

    ////////////////////////////////////////////////////////////////
    APIDCI_MM void *alloc(std::size_t size);
    APIDCI_MM void free(void *ptr);
    APIDCI_MM std::size_t size(void *ptr);
    APIDCI_MM std::size_t occupied();











    APIDCI_MM const Stack *stackAlloc();
    APIDCI_MM void stackFree(const Stack *stack);
    APIDCI_MM void stackCompact(const Stack *stack);

}}
