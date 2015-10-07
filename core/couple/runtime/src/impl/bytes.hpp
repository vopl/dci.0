#pragma once

#include <sys/uio.h>
#include <dci/couple/runtime/bytes/segment.hpp>

#include <string>

namespace dci { namespace couple { namespace runtime { namespace impl
{
    using namespace bytes;
    class Bytes
    {
        Bytes(const Bytes &) = delete;
        void operator=(const Bytes &) = delete;

    public:
        Bytes();
        Bytes(Bytes &&from);
        Bytes(std::size_t size, Segment *first, Segment *last);
        ~Bytes();

        Bytes &operator=(Bytes &&);

        Bytes clone() const;

        void append(Bytes &&bytes);
        void append(const char *str, std::size_t size);
        void append(const byte *data, std::size_t size);
        void enlargeAtLeast(std::size_t size);

        bool empty() const;
        std::size_t size() const;
        byte &operator[](std::size_t index) const;

        void clear();

        std::size_t segmentsAmount() const;
        void fillIovec(iovec *iov) const;

        void dropFirst(std::size_t size);
        void dropLast(std::size_t size);

        Bytes detachFirst(std::size_t size);
        Bytes detachLast(std::size_t size);

        void fillAndDropFirst(byte *data, std::size_t size);
        void fillAndDropLast(byte *data, std::size_t size);

        std::string toString();

    private:
        static constexpr std::size_t _segmentSize = sizeof(Segment);

        std::size_t _size;
        Segment *_first;
        Segment *_last;
    };
}}}}
