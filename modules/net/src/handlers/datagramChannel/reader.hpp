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
    struct Reader
    {
        Reader();
        ~Reader();

        bool hasRequests() const;
        Future< Bytes> pushRequest();
        Future< Bytes, Address> pushRequest2();
        void pump(Descriptor &d);
        void close(const std::error_code &ec);

    private:
        struct Request
            : public dci::mm::NewDelete<Request>
        {
            enum class PromiseKind
            {
                data,
                dataAddress
            } _promiseKind;

            union PromiseHolder
            {
                dci::async::Promise<std::error_code, Bytes>             _d;
                dci::async::Promise<std::error_code, Bytes, Address>    _da;

                PromiseHolder();
                ~PromiseHolder();
            } _promiseHolder;

            Request *                                               _next{nullptr};

            Request(PromiseKind promiseKind);
            ~Request();
        };

        Request *_requestsFirst;
        Request *_requestsLast;
        std::size_t _requestsAmount;

    private:

    };



    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Reader<Address>::Reader()
        : _requestsFirst(nullptr)
        , _requestsLast(nullptr)
        , _requestsAmount(0)
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Reader<Address>::~Reader()
    {
        assert(!_requestsFirst);
        assert(!_requestsLast);
        assert(!_requestsAmount);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    bool Reader<Address>::hasRequests() const
    {
        return !!_requestsFirst;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Future< Bytes> Reader<Address>::pushRequest()
    {
        if(!_requestsFirst)
        {
            assert(!_requestsLast);
            assert(!_requestsAmount);

            Request *request = new Request(Request::PromiseKind::data);
            _requestsFirst = _requestsLast = request;
            _requestsAmount = 1;

            return request->_promiseHolder._d.future();
        }

        assert(_requestsLast);

        Request *request = new Request(Request::PromiseKind::data);
        _requestsLast->_next = request;
        _requestsLast = request;
        _requestsAmount++;

        return request->_promiseHolder._d.future();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Future< Bytes, Address> Reader<Address>::pushRequest2()
    {
        if(!_requestsFirst)
        {
            assert(!_requestsLast);
            assert(!_requestsAmount);

            Request *request = new Request(Request::PromiseKind::dataAddress);
            _requestsFirst = _requestsLast = request;
            _requestsAmount = 1;

            return request->_promiseHolder._da.future();
        }

        assert(_requestsLast);

        Request *request = new Request(Request::PromiseKind::dataAddress);
        _requestsLast->_next = request;
        _requestsLast = request;
        _requestsAmount++;

        return request->_promiseHolder._da.future();
    }

    namespace
    {
        static const std::size_t segmentsPerMessage = (1500 + bytes::Segment::_granula) / bytes::Segment::_granula;
        static const std::size_t messagesPrepared = 256;

        static std::vector<mmsghdr> mmsgs(messagesPrepared);
        static std::vector<Bytes> buffers(messagesPrepared);
        static std::vector<iovec[segmentsPerMessage]> iovs(messagesPrepared);
        static std::vector<sockaddr_in6> addresses(messagesPrepared);

        bool regenerateBuffers(std::size_t amount)
        {
            for(std::size_t index(0); index<amount; ++index)
            {
                assert(buffers[index].empty());

                mmsghdr &h = mmsgs[index];

                h.msg_hdr.msg_name = &addresses[index];
                h.msg_hdr.msg_namelen = sizeof(sockaddr_in6);

                h.msg_hdr.msg_iov = iovs[index];
                h.msg_hdr.msg_iovlen = segmentsPerMessage;

                h.msg_hdr.msg_control = nullptr;
                h.msg_hdr.msg_controllen = 0;

                iovec *iov = (iovec *)h.msg_hdr.msg_iov + segmentsPerMessage;
                iov--;

                bytes::Segment *slast = new bytes::Segment(bytes::Segment::_granula);
                bytes::Segment *sfirst = slast;
                iov->iov_base = sfirst->_buffer;
                iov->iov_len = sfirst->_size;
                iov--;


                for(std::size_t sindex(1); sindex<segmentsPerMessage; ++sindex)
                {
                    sfirst = new bytes::Segment(bytes::Segment::_granula, 0, sfirst);
                    iov->iov_base = sfirst->_buffer;
                    iov->iov_len = sfirst->_size;
                    iov--;
                }

                buffers[index] = Bytes(segmentsPerMessage * bytes::Segment::_granula, sfirst, slast);
            }

            return true;
        }

        static bool stub = regenerateBuffers(messagesPrepared);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    void Reader<Address>::pump(Descriptor &d)
    {
        static_assert(sizeof(typename utils::AddressSpares<Address>::SockAddr) <= sizeof(sockaddr_in6), "addresses buffer too small");
        assert(dci::poll::Descriptor::rsf_read & d.readyState());

        while(_requestsFirst)
        {
            ssize_t res = ::recvmmsg(d, mmsgs.data(), std::min(_requestsAmount, mmsgs.size()), 0, nullptr);

            assert(0 != res);

            if(0 < res)
            {
                for(std::size_t index(0); index < (std::size_t)res; ++index)
                {
                    Request *request = _requestsFirst;
                    _requestsFirst = _requestsFirst->_next;

                    mmsghdr &h = mmsgs[index];

                    Bytes b = std::move(buffers[index]);
                    b.dropLast(b.size() - h.msg_len);

                    if(Request::PromiseKind::data == request->_promiseKind)
                    {
                        request->_promiseHolder._d.resolveValue(std::move(b));
                    }
                    else
                    {
                        Address a;
                        utils::fillSockaddr(*(typename utils::AddressSpares<Address>::SockAddr *)&addresses[index], a);
                        request->_promiseHolder._da.resolveValue(std::move(b), std::move(a));
                    }

                    delete request;
                }

                _requestsAmount -= res;
                regenerateBuffers(res);

                if(!_requestsFirst)
                {
                    _requestsLast = nullptr;
                }
                else if((std::size_t)res < mmsgs.size())
                {
                    d.resetReadyState(Descriptor::rsf_write);
                }
                else
                {
                    continue;
                }
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
    void Reader<Address>::close(const std::error_code &ec)
    {
        while(_requestsFirst)
        {
            Request *request = _requestsFirst;
            _requestsFirst = _requestsFirst->_next;

            if(Request::PromiseKind::data == request->_promiseKind)
            {
                request->_promiseHolder._d.resolveError(std::error_code(ec));
            }
            else
            {
                request->_promiseHolder._da.resolveError(std::error_code(ec));
            }

            delete request;
        }

        _requestsLast = nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Reader<Address>::Request::PromiseHolder::PromiseHolder()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Reader<Address>::Request::PromiseHolder::~PromiseHolder()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Reader<Address>::Request::Request(PromiseKind promiseKind)
        : _promiseKind(promiseKind)
    {
        if(Request::PromiseKind::data == _promiseKind)
        {
            new (&_promiseHolder._d) dci::async::Promise<std::error_code, Bytes>();
        }
        else
        {
            new (&_promiseHolder._da) dci::async::Promise<std::error_code, Bytes, Address>();
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Reader<Address>::Request::~Request()
    {
        if(Request::PromiseKind::data == _promiseKind)
        {
            using P = dci::async::Promise<std::error_code, Bytes>;
            _promiseHolder._d.~P();
        }
        else
        {
            using P = dci::async::Promise<std::error_code, Bytes, Address>;
            _promiseHolder._da.~P();
        }
    }

}}
