#include "writer.hpp"
#include <sys/uio.h>

namespace handlers { namespace streamChannel
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Writer::Request::Request(std::size_t tailSize)
        : _tailSize {tailSize}
        , _promise {}
        , _next {nullptr}
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Writer::Writer()
        : _requestsFirst {nullptr}
        , _requestsLast {nullptr}
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Writer::~Writer()
    {
        assert(!_requestsFirst);
        assert(!_requestsLast);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool Writer::hasRequests() const
    {
        return !!_requestsFirst;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< > Writer::pushRequest(Bytes &&b)
    {
        if(!_requestsFirst)
        {
            assert(!_requestsLast);

            Request *request = new Request {b.size()};

            assert(_buffer.empty());
            _buffer = std::forward<Bytes>(b);

            _requestsFirst = _requestsLast = request;
            return request->_promise.future();
        }

        assert(_requestsLast);

        Request *request = new Request {b.size()};

        assert(!_buffer.empty());
        _buffer.append(std::forward<Bytes>(b));

        _requestsLast->_next = request;
        _requestsLast = request;

        return request->_promise.future();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Writer::pump(Descriptor &d)
    {
        assert(dci::poll::Descriptor::rsf_write & d.readyState());

        int iovAmount = _buffer.segmentsAmount();
        if(iovAmount)
        {
            iovec *iov = (iovec *)alloca(iovAmount * sizeof(iovec));
            _buffer.fillIovec(iov);

            ssize_t res = ::writev(d, iov, iovAmount);

            if(0 > res)
            {
                close(systemError());
                return;
            }

            std::size_t writedSize = res;

            _buffer.dropFirst(writedSize);

            for(;;)
            {
                if(!_requestsFirst)
                {
                    _requestsLast = nullptr;
                    break;
                }

                if(_requestsFirst->_tailSize <= writedSize)
                {
                    Request *request = _requestsFirst;
                    _requestsFirst = _requestsFirst->_next;

                    writedSize -= request->_tailSize;
                    request->_promise.resolveValue();
                    delete request;

                    continue;
                }

                if(!writedSize)
                {
                    d.resetReadyState(Descriptor::rsf_write);
                    break;
                }

                _requestsFirst->_tailSize -= writedSize;
            }
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Writer::close(const std::error_code &ec)
    {
        _buffer.clear();

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
