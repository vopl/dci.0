#pragma once

#include <dci/couple/runtime.hpp>
#include <dci/poll.hpp>
#include <dci/mm.hpp>

#include "../../utils/sockaddr.hpp"

#include <sys/types.h>
#include <sys/socket.h>

namespace handlers { namespace datagramChannel
{
    using namespace dci::couple::runtime;
    using namespace dci::poll;

    template <class Address>
    struct Writer
    {
        Writer();
        ~Writer();

        bool hasRequests() const;
        Future< > pushRequest2(Bytes &&b, Address &&a);
        void pump(int d, std::uint_fast32_t &readyState);
        void close(const std::error_code &ec);

    private:
        struct Request
            : public dci::mm::NewDelete<Request>
        {
            Bytes                                               _bytes;

            static const std::size_t                            _iovInlineAmount = 4;
            iovec                                               _iovInline[_iovInlineAmount];

            std::size_t                                         _iovAmount;
            iovec *                                             _iov;

            typename utils::AddressSpares<Address>::SockAddr    _sockaddr;
            dci::async::Promise<std::error_code>                _promise;

            Request *                                            _next{nullptr};

            Request(Bytes &&b, Address &&a);
            ~Request();
        };

        Request *_requestsFirst{nullptr};
        Request *_requestsLast{nullptr};

        std::vector<mmsghdr> _mmsgs;
    };



    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Writer<Address>::Writer()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Writer<Address>::~Writer()
    {
        assert(!_requestsFirst);
        assert(!_requestsLast);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    bool Writer<Address>::hasRequests() const
    {
        return !!_requestsFirst;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Future< > Writer<Address>::pushRequest2(Bytes &&b, Address &&a)
    {
        Request *request = new Request(std::move(b), std::move(a));

        if(!_requestsFirst)
        {
            assert(!_requestsLast);

            _requestsFirst = _requestsLast = request;
        }
        else
        {
            assert(_requestsLast);
            _requestsLast->_next = request;
            _requestsLast = request;
        }

        _mmsgs.resize(_mmsgs.size()+1);
        mmsghdr &mmsg = _mmsgs.back();

        mmsg.msg_hdr.msg_iov = request->_iov;
        mmsg.msg_hdr.msg_iovlen = request->_iovAmount;

        mmsg.msg_hdr.msg_name = &request->_sockaddr;
        mmsg.msg_hdr.msg_namelen = sizeof(request->_sockaddr);

        return request->_promise.future();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    void Writer<Address>::pump(int d, std::uint_fast32_t &readyState)
    {
        assert(dci::poll::Descriptor::rsf_write & readyState);

        while(_requestsFirst)
        {
            assert(!_mmsgs.empty());

            ssize_t res = ::sendmmsg(d, _mmsgs.data(), _mmsgs.size(), 0);

            assert(0 != res);

            if(0 < res)
            {
                _mmsgs.erase(_mmsgs.begin(), _mmsgs.begin() + res);
                while(res)
                {
                    Request *request = _requestsFirst;
                    _requestsFirst = _requestsFirst->_next;

                    request->_promise.resolveValue();
                    delete request;

                    res--;
                }

                if(!_requestsFirst)
                {
                    _requestsLast = nullptr;
                }
                else
                {
                    readyState &= ~Descriptor::rsf_write;
                }

                break;
            }
            else
            {
                close(systemError());
                return;
            }
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    void Writer<Address>::close(const std::error_code &ec)
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
    template <class Address>
    Writer<Address>::Request::Request(Bytes &&b, Address &&a)
        : _bytes(std::move(b))
        , _iovAmount(_bytes.segmentsAmount())
    {
        utils::fillSockaddr(a, _sockaddr);

        if(_iovAmount > _iovInlineAmount)
        {
            _iov = new iovec[_iovAmount];
        }
        else
        {
            _iov = _iovInline;
        }

        _bytes.fillIovec(_iov);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Writer<Address>::Request::~Request()
    {
        if(_iov != _iovInline)
        {
            delete[] _iov;
        }
    }

}}
