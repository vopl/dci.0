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

        Future< Bytes> pushRequest();
        void pump(Descriptor &d);
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
        static Bytes _buffer;
    };
}}
