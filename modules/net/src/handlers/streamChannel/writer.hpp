#pragma once

#include <dci/couple/runtime.hpp>
#include <dci/poll.hpp>

namespace handlers { namespace streamChannel
{
    using namespace dci::couple::runtime;
    using namespace dci::poll;

    struct Writer
    {
        Writer();
        ~Writer();

        bool hasRequests() const;
        Future< > pushRequest(Bytes &&b);
        void pump(int d, std::uint_fast32_t &readyState);
        void close(const std::error_code &ec);

    private:
        Bytes _buffer;

        struct Request
            : public dci::mm::NewDelete<Request>
        {
            std::size_t _tailSize {0};
            dci::async::Promise<std::error_code> _promise;
            Request *_next {nullptr};

            Request(std::size_t tailSize);
        };

        Request *_requestsFirst;
        Request *_requestsLast;
    };
}}
