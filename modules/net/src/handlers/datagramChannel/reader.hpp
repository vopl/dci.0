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
        void pump(int d, std::uint_fast32_t &readyState);
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

        using BufferSegment = std::unique_ptr<bytes::Segment>;
        class Buffer
        {
            BufferSegment _segments[segmentsPerMessage];

        public:
            void ensureFull(iovec *iov)
            {
                for(std::size_t sindex(0); sindex<segmentsPerMessage; ++sindex)
                {
                    BufferSegment &bs = _segments[sindex];

                    assert(!bs);
                    bs.reset(new bytes::Segment(bytes::Segment::_granula));

                    iov->iov_base = bs->_buffer;
                    iov->iov_len = bs->_size;
                    iov++;
                }
            }

            Bytes emitBytes(std::size_t size, iovec *iov)
            {
                assert(size <= segmentsPerMessage * bytes::Segment::_granula);

                if(!size)
                {
                    return Bytes();
                }

                BufferSegment *bs = _segments;

                bytes::Segment *first = bs->release();

                BufferSegment(new bytes::Segment(bytes::Segment::_granula)).swap(*bs);
                iov->iov_base = bs->get()->_buffer;
                //iov->iov_len = bs->get()->_size;
                assert(bytes::Segment::_granula == iov->iov_len);

                iov++;
                bs++;

                bytes::Segment *last = first;
                std::size_t emittedSize(first->_granula);

                while(emittedSize < size)
                {
                    last->_next = bs->release();
                    BufferSegment(new bytes::Segment(bytes::Segment::_granula)).swap(*bs);
                    iov->iov_base = bs->get()->_buffer;
                    //iov->iov_len = bs->get()->_size;
                    assert(bytes::Segment::_granula == iov->iov_len);

                    iov++;
                    bs++;

                    last = last->_next;
                    emittedSize += last->_granula;
                }

                if(emittedSize > size)
                {
                    last->_size -= emittedSize - size;
                }

                return Bytes(size, first, last);
            }
        };
        static std::vector<Buffer> buffers(messagesPrepared);

        static std::vector<iovec[segmentsPerMessage]> iovs(messagesPrepared);
        static std::vector<sockaddr_in6> addresses(messagesPrepared);


        static volatile bool stub = ([]()->bool
        {
            for(std::size_t index(0); index<messagesPrepared; ++index)
            {
                mmsghdr &h = mmsgs[index];

                h.msg_hdr.msg_name = &addresses[index];
                h.msg_hdr.msg_namelen = sizeof(sockaddr_in6);

                h.msg_hdr.msg_iov = iovs[index];
                h.msg_hdr.msg_iovlen = segmentsPerMessage;

                h.msg_hdr.msg_control = nullptr;
                h.msg_hdr.msg_controllen = 0;

                buffers[index].ensureFull(h.msg_hdr.msg_iov);
            }
            return true;
        })();


    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    void Reader<Address>::pump(int d, std::uint_fast32_t &readyState)
    {
        static_assert(sizeof(typename utils::AddressSpares<Address>::SockAddr) <= sizeof(sockaddr_in6), "addresses buffer too small");
        assert(dci::poll::Descriptor::rsf_read & readyState);

        while(_requestsFirst)
        {
            ssize_t res = ::recvmmsg(d, mmsgs.data(), static_cast<unsigned int>(std::min(_requestsAmount, mmsgs.size())), 0, nullptr);

            assert(0 != res);

            if(0 < res)
            {
                for(std::size_t index(0); index < static_cast<std::size_t>(res); ++index)
                {
                    Request *request = _requestsFirst;
                    _requestsFirst = _requestsFirst->_next;

                    mmsghdr &h = mmsgs[index];

                    if(Request::PromiseKind::data == request->_promiseKind)
                    {
                        request->_promiseHolder._d.resolveValue(buffers[index].emitBytes(h.msg_len, h.msg_hdr.msg_iov));
                    }
                    else
                    {
                        Address a;
                        utils::fillSockaddr(*reinterpret_cast<typename utils::AddressSpares<Address>::SockAddr *>(&addresses[index]), a);
                        request->_promiseHolder._da.resolveValue(
                                    buffers[index].emitBytes(h.msg_len, h.msg_hdr.msg_iov),
                                    std::move(a));
                    }

                    delete request;
                }

                _requestsAmount -= static_cast<std::size_t>(res);

                if(!_requestsFirst)
                {
                    _requestsLast = nullptr;
                }
                else if(static_cast<std::size_t>(res) < mmsgs.size())
                {
                    readyState &= ~Descriptor::rsf_write;
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
