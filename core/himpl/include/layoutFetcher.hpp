#pragma once

#include <cstddef>
#include <type_traits>

namespace dci { namespace himpl
{
    namespace details
    {
        template <class T>
        class EmptyProbe : T {char c;};
    }

    template <class T, bool isEmpty = (1 == sizeof(T) && 1 == sizeof(details::EmptyProbe<T>))>
    struct LayoutFetcher
    {
        static const std::size_t _size = sizeof(T);
        static const std::size_t _align = alignof(T);
        static const bool _polymorphic = std::is_polymorphic<T>::value;
    };

    template <class T>
    struct LayoutFetcher<T, true>
    {
        static const std::size_t _size = 0;
        static const std::size_t _align = alignof(T);
        static const bool _polymorphic = std::is_polymorphic<T>::value;
    };

}}
