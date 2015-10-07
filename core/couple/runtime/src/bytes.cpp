#include <dci/couple/runtime/bytes.hpp>
#include "impl/bytes.hpp"
#include <cstring>

namespace dci { namespace couple { namespace runtime
{
    Bytes::Bytes()
        : himpl::FaceLayout<impl::Bytes>()
    {
    }

    Bytes::Bytes(Bytes &&from)
        : himpl::FaceLayout<impl::Bytes>(std::forward<himpl::FaceLayout<impl::Bytes>>(from))
    {
    }

    Bytes::Bytes(impl::Bytes &&from)
        : himpl::FaceLayout<impl::Bytes>(std::forward<impl::Bytes>(from))
    {
    }

    Bytes::Bytes(std::size_t size, bytes::Segment *first, bytes::Segment *last)
        : himpl::FaceLayout<impl::Bytes>(size, first, last)
    {

    }

    Bytes::~Bytes()
    {
    }

    Bytes &Bytes::operator=(Bytes &&from)
    {
        impl() = std::move(from.impl());
        return *this;
    }

    Bytes Bytes::clone() const
    {
        return impl().clone();
    }

    bool Bytes::empty() const
    {
        return impl().empty();
    }

    std::size_t Bytes::size() const
    {
        return impl().size();
    }

    byte &Bytes::operator[](std::size_t index) const
    {
        return impl()[index];
    }

    void Bytes::append(Bytes &&data)
    {
        return impl().append(std::move(data.impl()));
    }

    void Bytes::append(const char *str)
    {
        return impl().append(str, strlen(str));
    }

    void Bytes::append(const char *str, std::size_t size)
    {
        return impl().append(str, size);
    }

    void Bytes::append(const byte *data, std::size_t size)
    {
        return impl().append(data, size);
    }

    void Bytes::enlargeAtLeast(std::size_t size)
    {
        return impl().enlargeAtLeast(size);
    }

    void Bytes::clear()
    {
        return impl().clear();
    }

    Bytes &Bytes::operator<<(const char *str)
    {
        impl().append(str, strlen(str));
        return *this;
    }

    std::size_t Bytes::segmentsAmount() const
    {
        return impl().segmentsAmount();
    }

    void Bytes::fillIovec(iovec *iov) const
    {
        return impl().fillIovec(iov);
    }

    void Bytes::dropFirst(std::size_t size)
    {
        return impl().dropFirst(size);
    }

    void Bytes::dropLast(std::size_t size)
    {
        return impl().dropLast(size);
    }

    Bytes Bytes::detachFirst(std::size_t size)
    {
        return impl().detachFirst(size);
    }

    Bytes Bytes::detachLast(std::size_t size)
    {
        return impl().detachLast(size);
    }

    void Bytes::fillAndDropFirst(byte *data, std::size_t size)
    {
        return impl().fillAndDropFirst(data, size);
    }

    void Bytes::fillAndDropLast(byte *data, std::size_t size)
    {
        return impl().fillAndDropLast(data, size);
    }

    std::string Bytes::toString()
    {
        return impl().toString();
    }

}}}
