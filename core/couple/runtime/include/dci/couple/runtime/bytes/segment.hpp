#pragma once

#include <dci/mm/newDelete.hpp>
#include <cstdint>
#include <cassert>

namespace dci { namespace couple { namespace runtime { namespace bytes
{
    using byte = std::uint8_t;

    struct Segment
        : public mm::NewDelete<Segment>
    {
        Segment(std::uint32_t size = 0, std::uint32_t offset = 0, Segment *next=nullptr);

        static constexpr std::size_t _granula = 1024;

        byte _buffer[_granula];

        std::uint32_t _size;
        std::uint32_t _offset;
        Segment *_next;
    };



    inline Segment::Segment(std::uint32_t size, std::uint32_t offset, Segment *next)
        : _size {size}
        , _offset {offset}
        , _next {next}
    {
        assert(_offset+_size <= _granula);
    }


}}}}
