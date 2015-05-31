#pragma once

#include <sys/uio.h>
#include "bytes/segment.hpp"

#include <string>

namespace dci { namespace couple { namespace runtime { namespace impl
{

    class Bytes
    {
        Bytes(const Bytes &) = delete;
        void operator=(const Bytes &) = delete;

    public:
        Bytes();
        Bytes(Bytes &&from);
        Bytes(std::size_t size, bytes::Segment *first, bytes::Segment *last);
        ~Bytes();

        Bytes &operator=(Bytes &&);

        void append(Bytes &&bytes);
        void append(const char *str, std::size_t size);
        void enlargeAtLeast(std::size_t size);

        bool empty() const;
        std::size_t size() const;

        void clear();

        std::size_t segmentsAmount() const;
        void fillIovec(iovec *iov) const;

        void dropFirst(std::size_t size);
        void dropLast(std::size_t size);

        Bytes detachFirst(std::size_t size);
        Bytes detachLast(std::size_t size);

        std::string toString();

    private:
        static constexpr std::size_t _segmentSize = sizeof(bytes::Segment);

        std::size_t _size;
        bytes::Segment *_first;
        bytes::Segment *_last;
    };
}}}}
