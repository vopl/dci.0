#include "reader.hpp"
#include <sys/uio.h>

namespace handlers { namespace streamChannel
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Reader::Reader()
        : _requestsFirst {nullptr}
        , _requestsLast {nullptr}
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Reader::~Reader()
    {
        assert(!_requestsFirst);
        assert(!_requestsLast);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool Reader::hasRequests() const
    {
        return !!_requestsFirst;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< Bytes> Reader::pushRequest()
    {
        if(!_requestsFirst)
        {
            assert(!_requestsLast);

            Request *request = new Request;
            _requestsFirst = _requestsLast = request;

            return request->_promise.future();
        }

        assert(_requestsLast);

        Request *request = new Request;
        _requestsLast->_next = request;
        _requestsLast = request;

        return request->_promise.future();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Reader::pump(Descriptor &d)
    {
        assert(_requestsFirst);
        if(!_requestsFirst)
        {
            return;
        }

        assert(dci::poll::Descriptor::rsf_read & d.readyState());

        static const std::size_t bufLen = 1024*2;
        std::pair<iovec *, std::size_t> buf = _preparedBuffer.prepare(bufLen);

        ssize_t res = ::readv(d, buf.first, buf.second);

        if(0 > res)
        {
            close(systemError());
            return;
        }
        assert((std::size_t)res <= bufLen);

        Request *request = _requestsFirst;
        _requestsFirst = _requestsFirst->_next;
        if(!_requestsFirst)
        {
            _requestsLast = nullptr;
        }

        if((std::size_t)res < bufLen)
        {
            d.resetReadyState(Descriptor::rsf_read);
        }

        request->_promise.resolveValue(_preparedBuffer.flush(res));

        delete request;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Reader::close(const std::error_code &ec)
    {
        while(_requestsFirst)
        {
            Request *request = _requestsFirst;
            _requestsFirst = _requestsFirst->_next;

            request->_promise.resolveError(std::error_code(ec));

            delete request;
        }

        _requestsLast = nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Reader::PreparedBuffer::PreparedBuffer()
        : _segmentFirst {new bytes::Segment(bytes::Segment::_granula)}
        , _segmentLast {_segmentFirst}
        , _iovecs {1, iovec {_segmentLast->_buffer, bytes::Segment::_granula}}
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Reader::PreparedBuffer::~PreparedBuffer()
    {
        while(_segmentFirst)
        {
            bytes::Segment *segment = _segmentFirst;
            _segmentFirst = _segmentFirst->_next;
            delete segment;
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    std::pair<iovec *, std::size_t> Reader::PreparedBuffer::prepare(std::size_t size)
    {
        size /= bytes::Segment::_granula;

        //один или два лишних всегда
        while(size >= _iovecs.size())
        {
            _segmentLast->_next = new bytes::Segment(bytes::Segment::_granula);
            _segmentLast = _segmentLast->_next;
            _iovecs.emplace_back(iovec {_segmentLast->_buffer, bytes::Segment::_granula});
        }

        return std::make_pair(&_iovecs[0], _iovecs.size());
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Bytes Reader::PreparedBuffer::flush(std::size_t size)
    {
        if(!size)
        {
            return Bytes();
        }

        bytes::Segment *first = _segmentFirst;
        bytes::Segment *last = first;

        std::size_t segmentsAmount = (size % bytes::Segment::_granula) ?
                                              size/bytes::Segment::_granula+1:
                                              size/bytes::Segment::_granula;

        for(std::size_t cnt{1}; cnt<segmentsAmount; cnt++)
        {
            last = last->_next;
        }
        _segmentFirst = last->_next;

        last->_next = nullptr;
        last->_size = size - (segmentsAmount-1) * bytes::Segment::_granula;
        _iovecs.erase(_iovecs.begin(), _iovecs.begin()+segmentsAmount);

        return Bytes(size, first, last);
    }

    Reader::PreparedBuffer Reader::_preparedBuffer;
}}
