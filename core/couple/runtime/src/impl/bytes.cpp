#include "bytes.hpp"

#include <cstring>

namespace dci { namespace couple { namespace runtime { namespace impl
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Bytes::Bytes()
        : _size {0}
        , _first {nullptr}
        , _last {nullptr}
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Bytes::Bytes(Bytes &&from)
        : _size {from._size}
        , _first {from._first}
        , _last {from._last}
    {
        from._size = 0;
        from._first = nullptr;
        from._last = nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Bytes::Bytes(std::size_t size, bytes::Segment *first, bytes::Segment *last)
        : _size {size}
        , _first {first}
        , _last {last}
    {
        assert(!_size || !_last->_next);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Bytes::~Bytes()
    {
        clear();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
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

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Bytes Bytes::clone() const
    {
        bytes::Segment *first, *last;
        if(_size)
        {
            bytes::Segment *src = _first;
            bytes::Segment *iter = new bytes::Segment(src->_size, src->_offset);

            first = iter;

            for(;;)
            {
                memcpy(iter->_buffer+iter->_offset, src->_buffer+src->_offset, src->_size);
                src = src->_next;
                if(src)
                {
                    iter->_next = new bytes::Segment(src->_size, src->_offset);
                    iter = iter->_next;
                }
                else
                {
                    break;
                }
            }

            last = iter;
        }
        else
        {
            first = last = nullptr;
        }

        return Bytes(_size, first, last);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
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

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Bytes::append(const char *str, std::size_t size)
    {
        if(!size)
        {
            return;
        }

        if(!_last)
        {
            assert(!_first);
            assert(!_size);
            _first = _last = new bytes::Segment;
        }

        _size += size;

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

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Bytes::enlargeAtLeast(std::size_t size)
    {
        size += _size;

        if(!_last)
        {
            assert(!_first);
            assert(!_size);

            _first = _last = new bytes::Segment(bytes::Segment::_granula);
            _size = bytes::Segment::_granula;
        }

        bytes::Segment *cur = _last;
        while(size > _size)
        {
            bytes::Segment *next = new bytes::Segment(bytes::Segment::_granula);
            assert(!next->_next);
            _size += bytes::Segment::_granula;
            assert(!cur->_next);
            cur->_next = next;
            cur = next;
        }

        _last = cur;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool Bytes::empty() const
    {
        return _size ? false : true;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    std::size_t Bytes::size() const
    {
        return _size;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
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

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    std::size_t Bytes::segmentsAmount() const
    {
        std::size_t res {0};
        for(bytes::Segment *iter = _first; iter; iter=iter->_next)
        {
            res++;
        }

        return res;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Bytes::fillIovec(iovec *iov) const
    {
        for(bytes::Segment *iter = _first; iter; iter=iter->_next)
        {
            iov->iov_base = &iter->_buffer[iter->_offset];
            iov->iov_len = iter->_size;
            iov++;
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
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

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Bytes::dropLast(std::size_t size)
    {
        if(size >= _size)
        {
            clear();
            return;
        }

        bytes::Segment *iter = _first;

        std::size_t keepSize = _size - size;
        while(iter->_size <= keepSize)
        {
            _last = iter;
            keepSize -= iter->_size;
            iter = iter->_next;
        }

        if(keepSize)
        {
            _last = iter;
            iter->_size = keepSize;
            iter = iter->_next;
        }

        _last->_next = nullptr;

        while(iter)
        {
            bytes::Segment *cur = iter;
            iter = iter->_next;
            delete cur;
        }

        _size -= size;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
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
                detachBound->_next = nullptr;
                return impl::Bytes {size, detachFirst, detachBound};
            }
        }

        assert(!"never here");
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Bytes Bytes::detachLast(std::size_t size)
    {
        (void)size;
        assert(0);

        return Bytes();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
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
