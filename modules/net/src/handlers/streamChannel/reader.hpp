#pragma once

#include <dci/couple/runtime.hpp>
#include <dci/poll.hpp>
#include <dci/mm.hpp>

namespace handlers { namespace streamChannel
{
    using namespace dci::couple::runtime;
    using namespace dci::poll;

    struct Reader
    {
        Reader();
        ~Reader();

        bool hasRequests() const;
        Future< Bytes> pushRequest();
        void pump(int d, std::uint_fast32_t &readyState);
        void close(const std::error_code &ec);

    private:
        struct Request
            : public dci::mm::NewDelete<Request>
        {
            Request *_next{nullptr};

            dci::async::Promise<std::error_code, Bytes> _promise;
        };

        Request *_requestsFirst;
        Request *_requestsLast;

    private:
        class PreparedBuffer
        {
        public:
            PreparedBuffer();
            ~PreparedBuffer();
            std::pair<iovec *, std::size_t> prepare(std::size_t size);
            Bytes flush(std::size_t size);

        private:
            bytes::Segment *    _segmentFirst;
            bytes::Segment *    _segmentLast;
            std::vector<iovec>  _iovecs;
        };

        static PreparedBuffer _preparedBuffer;
    };
}}
