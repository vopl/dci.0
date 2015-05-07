#include "host.hpp"
#include "interface.hpp"

#include "datagramHost.hpp"
#include "streamHost.hpp"

namespace handlers
{
    Future<list<Interface>> HostHandler::interfaces()
    {
        InterfaceHandler* i = new InterfaceHandler;
        this->interfaceAdded(std::forward<Interface>(*i));

        //do impl
        assert(0);

        list<Interface> l;

        return std::move(l);
    }

    Future< ip4::stream::Host> HostHandler::ip4StreamHost()
    {
        auto *v = new StreamHostHandler<ip4::Address>;
        return ip4::stream::Host(*v);
    }

    Future< ip4::datagram::Host> HostHandler::ip4DatagramHost()
    {
        auto *v = new DatagramHostHandler<ip4::Address>;
        return ip4::datagram::Host(*v);
    }

    Future< ip6::stream::Host> HostHandler::ip6StreamHost()
    {
        auto *v = new StreamHostHandler<ip6::Address>;
        return ip6::stream::Host(*v);
    }

    Future< ip6::datagram::Host> HostHandler::ip6DatagramHost()
    {
        auto *v = new DatagramHostHandler<ip6::Address>;
        return ip6::datagram::Host(*v);
    }

    Future< local::stream::Host> HostHandler::localStreamHost()
    {
        auto *v = new StreamHostHandler<local::Address>;
        return local::stream::Host(*v);
    }

    void HostHandlerFactory::createService(void *outFuture)
    {
        auto *res = static_cast<dci::async::Future<std::error_code, Host> *>(outFuture);
        *res = Host(* new HostHandler);
    }

}
