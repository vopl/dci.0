#pragma once

#include <cstdint>

namespace spares { namespace protocol
{
    using MessageType = std::uint32_t;
    constexpr MessageType _nullMessageType = 0;
}}
