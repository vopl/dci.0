#pragma once

#include <dci/himpl/faceLayout.hpp>
#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE(<dci/couple/runtime/layoutProvider.hpp>)

#include "bytes/segment.hpp"
#include "api.hpp"
#include <string>

struct iovec;

namespace dci { namespace couple { namespace runtime
{
    namespace impl
    {
        class Bytes;
    }

    using byte = bytes::byte;

    class APIDCI_COUPLE_RUNTIME Bytes
        : public himpl::FaceLayout<impl::Bytes>
    {
    private:
        Bytes(impl::Bytes &&);
        Bytes(const Bytes &) = delete;
        void operator=(const Bytes &) = delete;

    public:
        Bytes();
        Bytes(Bytes &&);
        Bytes(std::size_t size, bytes::Segment *first, bytes::Segment *last);
        ~Bytes();

        Bytes &operator=(Bytes &&);

        Bytes clone() const;

        bool empty() const;
        std::size_t size() const;
        byte &operator[](std::size_t index) const;

        void append(Bytes &&data);
        void append(const char *str);
        void append(const char *str, std::size_t size);
        void append(const byte *data, std::size_t size);
        void enlargeAtLeast(std::size_t size);

        void clear();

        Bytes &operator<<(const char *str);

        std::size_t segmentsAmount() const;
        void fillIovec(iovec *iov) const;

        void dropFirst(std::size_t size);
        void dropLast(std::size_t size);

        Bytes detachFirst(std::size_t size);
        Bytes detachLast(std::size_t size);

        void fillAndDropFirst(byte *data, std::size_t size);
        void fillAndDropLast(byte *data, std::size_t size);

        std::string toString();
    };

}}}
