#pragma once

#include "runtime/interface.hpp"
#include "runtime/signal.hpp"
#include "runtime/wire.hpp"
#include "runtime/errcCategoryProvider.hpp"
#include "runtime/errcInitializer.hpp"

#include "runtime/bytes.hpp"
#include <dci/async/future.hpp>

#include <cstdint>
#include <climits>
#include <string>
#include <array>
#include <vector>
#include <memory>

namespace dci { namespace couple { namespace runtime
{
    using void_     = void;
    using bool_     = bool;
    using string    = std::string;
    using int8      = std::int8_t;
    using int16     = std::int16_t;
    using int32     = std::int32_t;
    using int64     = std::int64_t;
    using uint8     = std::uint8_t;
    using uint16    = std::uint16_t;
    using uint32    = std::uint32_t;
    using uint64    = std::uint64_t;
    using real32    = float;
    using real64    = double;

    static_assert(32 == CHAR_BIT*sizeof(real32), "wrong real32 type");
    static_assert(64 == CHAR_BIT*sizeof(real64), "wrong real64 type");

    template <class T, std::size_t N>
    using array = std::array<T, N>;

    template <class T>
    using list = std::vector<T>;

    template <class K, class V>
    using map = std::map<K, V>;

    template <class... TT>
    struct variant
    {

    };

    template <class... T>
    using Future = ::dci::async::Future< std::error_code, T...>;

}}}
