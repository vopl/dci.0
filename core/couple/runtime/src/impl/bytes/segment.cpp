#include "segment.hpp"

namespace dci { namespace couple { namespace runtime { namespace impl { namespace bytes
{
    Segment::Segment(std::uint32_t size, std::uint32_t offset, Segment *next)
        : _size {size}
        , _offset {offset}
        , _next {next}
    {
        assert(_offset+_size <= _granula);
    }

}}}}}
