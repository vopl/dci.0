#include "host.hpp"
#include "link.hpp"

#include "streamServer.hpp"
#include "streamClient.hpp"

#include "datagramChannel.hpp"

#include "../impl/host.hpp"
#include "../impl/link.hpp"

namespace handlers
{
    Host::Host(::impl::Host *impl)
        : _impl(impl)
    {
        if(_impl)
        {
            _impl->registerHandler(this);
        }
    }

    Host::~Host()
    {
        if(_impl)
        {
            _impl->unregisterHandler(this);
        }
    }

    void Host::dropImpl()
    {
        if(_impl)
        {
            _impl = nullptr;
        }
    }

    Future<map<uint32, ::net::Link>> Host::links()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        map<uint32, ::net::Link> res;
        for(const auto &i : _impl->links())
        {
            res.emplace(i.first, * new Link(i.second.get()));
        }

        return std::move(res);
    }

    Future< ip4::stream::Server> Host::ip4StreamServer()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        auto *v = new StreamServer<ip4::Address>;
        return ip4::stream::Server(*v);
    }

    Future< ip4::stream::Client> Host::ip4StreamClient()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        auto *v = new StreamClient<ip4::Address>;
        return ip4::stream::Client(*v);
    }

    Future< ip4::datagram::Channel> Host::ip4DatagramChannel()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        auto *v = new DatagramChannel<ip4::Address>;
        return ip4::datagram::Channel(*v);
    }

    Future< ip6::stream::Server> Host::ip6StreamServer()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        auto *v = new StreamServer<ip6::Address>;
        return ip6::stream::Server(*v);
    }

    Future< ip6::stream::Client> Host::ip6StreamClient()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        auto *v = new StreamClient<ip6::Address>;
        return ip6::stream::Client(*v);
    }

    Future< ip6::datagram::Channel> Host::ip6DatagramChannel()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        auto *v = new DatagramChannel<ip6::Address>;
        return ip6::datagram::Channel(*v);
    }

    Future< local::stream::Server> Host::localStreamServer()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        auto *v = new StreamServer<local::Address>;
        return local::stream::Server(*v);
    }

    Future< local::stream::Client> Host::localStreamClient()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        auto *v = new StreamClient<local::Address>;
        return local::stream::Client(*v);
    }

    void HostHandlerFactory::createService(void *outFuture)
    {
        auto *res = static_cast<dci::async::Future<std::error_code, ::net::Host> *>(outFuture);
        *res = ::net::Host(* new Host(::impl::Host::instance()));
    }

}
