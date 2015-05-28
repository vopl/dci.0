#include "bytes.hpp"

#include <cstring>

namespace dci { namespace couple { namespace runtime { namespace impl
{
    Bytes::Bytes()
        : _size {0}
        , _first {nullptr}
        , _last {nullptr}
    {
    }

    Bytes::Bytes(Bytes &&from)
        : _size {from._size}
        , _first {from._first}
        , _last {from._last}
    {
        from._size = 0;
        from._first = nullptr;
        from._last = nullptr;
    }

    Bytes::Bytes(std::size_t size, bytes::Segment *first, bytes::Segment *last)
        : _size {size}
        , _first {first}
        , _last {last}
    {
    }

    Bytes::~Bytes()
    {
        clear();
    }

    Bytes &Bytes::operator=(Bytes &&from)
    {
        _size = from._size;
        _first = from._first;
        _last = from._last;

        from._size = 0;
        from._first = nullptr;
        from._last = nullptr;

        return *this;
    }

    void Bytes::append(Bytes &&data)
    {
        if(!_size)
        {
            clear();

            _size = data._size;
            _first = data._first;
            _last = data._last;

            data._size = 0;
            data._first = nullptr;
            data._last = nullptr;
        }
        else
        {
            assert(_first && _last);
            assert(!_last->_next);

            if(data._size)
            {
                assert(data._first && data._last);
                assert(!data._last->_next);

                _last->_next = data._first;
                _last = data._last;
                _size += data._size;

                data._size = 0;
                data._first = nullptr;
                data._last = nullptr;
            }
            else
            {
                assert(!data._first && !data._last);
            }
        }
    }

    void Bytes::append(const char *str, std::size_t size)
    {
        if(!size)
        {
            return;
        }

        _size += size;

        if(!_last)
        {
            assert(!_first);
            _first = _last = new bytes::Segment;
        }

        bytes::Segment *cur = _last;
        for(;;)
        {
            assert(cur->_offset + cur->_size <= bytes::Segment::_granula);
            std::size_t space = bytes::Segment::_granula - (cur->_offset + cur->_size);

            if(space > size)
            {
                space = size;
            }

            memcpy(&cur->_buffer[cur->_offset + cur->_size], str, space);

            cur->_size += space;
            size -= space;

            if(size)
            {
                bytes::Segment *next = new bytes::Segment;
                cur->_next = next;
                cur = next;
            }
            else
            {
                break;
            }
        }

        if(_last != cur)
        {
            _last = cur;
        }
    }

    bool Bytes::empty() const
    {
        return _size ? false : true;
    }

    std::size_t Bytes::size() const
    {
        return _size;
    }

    void Bytes::clear()
    {
        bytes::Segment *segment = _first;
        while(segment)
        {
            bytes::Segment *cur = segment;
            segment = segment->_next;

            delete cur;
        }

        _first = _last = nullptr;
        _size = 0;
    }

    std::size_t Bytes::segmentsAmount() const
    {
        std::size_t res {0};
        for(bytes::Segment *iter = _first; iter; iter=iter->_next)
        {
            res++;
        }

        return res;
    }

    void Bytes::fillIovec(iovec *iov) const
    {
        for(bytes::Segment *iter = _first; iter; iter=iter->_next)
        {
            iov->iov_base = &iter->_buffer[iter->_offset];
            iov->iov_len = iter->_size;
            iov++;
        }
    }

    void Bytes::dropFirst(std::size_t size)
    {
        if(size >= _size)
        {
            clear();
            return;
        }

        bytes::Segment *iter = _first;

        for(;;)
        {
            bytes::Segment *cur = iter;

            if(size > cur->_size)
            {
                iter = iter->_next;
                size -= cur->_size;
                delete cur;
            }
            else if(size < cur->_size)
            {
                cur->_offset += size;
                cur->_size -= size;
                break;
            }
            else
            {
                iter = iter->_next;
                delete cur;
                break;
            }
        }

        _first = iter;
    }

    void Bytes::dropLast(std::size_t size)
    {
        (void)size;
        assert(0);
    }

    Bytes Bytes::detachFirst(std::size_t size)
    {
        if(!size)
        {
            return Bytes {};
        }

        if(size >= _size)
        {
            return Bytes {std::move(*this)};
        }

        bytes::Segment *detachBound = _first;
        std::size_t detachSize = detachBound->_size;

        for(;;)
        {
            if(detachSize < size)
            {
                detachBound = detachBound->_next;
                detachSize += detachBound->_size;
            }
            else if(detachSize > size)
            {
                //посеридине
                bytes::Segment *detachFirst = _first;

                std::size_t boundTailSize = detachSize - size;

                _first = new bytes::Segment {static_cast<std::uint32_t>(boundTailSize), 0, detachBound->_next};
                ::memcpy(_first->_buffer, &detachBound->_buffer[detachBound->_offset + detachBound->_size - boundTailSize], boundTailSize);
                _size -= size;

                detachBound->_size -= boundTailSize;
                detachBound->_next = nullptr;
                return impl::Bytes {size, detachFirst, detachBound};
            }
            else
            {
                //точно по границе
                bytes::Segment *detachFirst = _first;
                _size -= size;
                _first = detachBound->_next;
                return impl::Bytes {size, detachFirst, detachBound};
            }
        }

        assert(!"never here");
    }

    Bytes Bytes::detachLast(std::size_t size)
    {
        (void)size;
        assert(0);

        return Bytes();
    }

    std::string Bytes::toString()
    {
        std::string res;
        res.reserve(_size);

        bytes::Segment *segment = _first;
        while(segment)
        {
            res.append(&segment->_buffer[segment->_offset], &segment->_buffer[segment->_offset + segment->_size]);
            segment = segment->_next;
        }

        assert(res.size() == _size);

        return res;
    }

}}}}
