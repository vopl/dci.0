#pragma once

#include "dci/async/future.hpp"
#include <system_error>
#include <dci/io/stream.hpp>
#include <dci/io/endpoint.hpp>

namespace dci { namespace io { namespace impl
{

    class Client
    {
        Client(const Client &) = delete;
        void operator=(const Client &) = delete;

    public:
        Client();
        Client(Client &&from);
        ~Client();

    public:
        async::Future<std::error_code, dci::io::Stream> connect(const dci::io::Endpoint &endpoint);

    };
}}}
