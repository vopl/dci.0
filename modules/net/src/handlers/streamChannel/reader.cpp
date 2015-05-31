#include "reader.hpp"
#include <sys/uio.h>

namespace handlers { namespace streamChannel
{
    Bytes Reader::_buffer;

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

        static const std::size_t bufLen = 1024*64;
        if(_buffer.size() < bufLen)
        {
            _buffer.enlargeAtLeast(bufLen - _buffer.size());
        }

        int iovAmount = _buffer.segmentsAmount();
        iovec *iov = (iovec *)alloca(iovAmount * sizeof(iovec));
        _buffer.fillIovec(iov);

        ssize_t res = ::readv(d, iov, iovAmount);

        if(0 > res)
        {
            close(systemError());
            return;
        }
        assert((std::size_t)res <= _buffer.size());

        Request *request = _requestsFirst;
        _requestsFirst = _requestsFirst->_next;
        if(!_requestsFirst)
        {
            _requestsLast = nullptr;
        }

        if((std::size_t)res == _buffer.size())
        {
            request->_promise.resolveValue(std::move(_buffer));
        }
        else
        {
            d.resetReadyState(Descriptor::rsf_read);
            request->_promise.resolveValue(_buffer.detachFirst(res));
        }

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

}}
