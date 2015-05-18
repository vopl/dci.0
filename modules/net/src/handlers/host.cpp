#include "host.hpp"
#include "link.hpp"

#include "datagramHost.hpp"
#include "streamHost.hpp"

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

    Future< ip4::stream::Host> Host::ip4StreamHost()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        auto *v = new StreamHost<ip4::Address>;
        return ip4::stream::Host(*v);
    }

    Future< ip4::datagram::Host> Host::ip4DatagramHost()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        auto *v = new DatagramHost<ip4::Address>;
        return ip4::datagram::Host(*v);
    }

    Future< ip6::stream::Host> Host::ip6StreamHost()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        auto *v = new StreamHost<ip6::Address>;
        return ip6::stream::Host(*v);
    }

    Future< ip6::datagram::Host> Host::ip6DatagramHost()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        auto *v = new DatagramHost<ip6::Address>;
        return ip6::datagram::Host(*v);
    }

    Future< local::stream::Host> Host::localStreamHost()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        auto *v = new StreamHost<local::Address>;
        return local::stream::Host(*v);
    }

    void HostHandlerFactory::createService(void *outFuture)
    {
        auto *res = static_cast<dci::async::Future<std::error_code, ::net::Host> *>(outFuture);
        *res = ::net::Host(* new Host(::impl::Host::instance()));
    }

}
