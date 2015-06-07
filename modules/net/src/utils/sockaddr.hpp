#pragma once

#include "net.hpp"

#include <type_traits>
#include <netinet/in.h>
#include <sys/un.h>

namespace utils
{
    template <class Address>
    struct AddressSpares;

    template <>
    struct AddressSpares<net::ip4::Address>
    {
        static const int af = AF_INET;
        using SockAddr = sockaddr_in;
    };


    template <>
    struct AddressSpares<net::ip6::Address>
    {
        static const int af = AF_INET6;
        using SockAddr = sockaddr_in6;
    };

    template <>
    struct AddressSpares<net::local::Address>
    {
        static const int af = AF_LOCAL;
        using SockAddr = sockaddr_un;
    };

    void fillSockaddr(const net::ip4::Address &address, sockaddr_in &inetAddress);
    void fillSockaddr(const net::ip6::Address &address, sockaddr_in6 &inetAddress);
    void fillSockaddr(const net::local::Address &address, sockaddr_un &inetAddress);

    void fillSockaddr(const sockaddr_in &inetAddress, net::ip4::Address &address);
    void fillSockaddr(const sockaddr_in6 &inetAddress, net::ip6::Address &address);
    void fillSockaddr(const sockaddr_un &inetAddress, net::local::Address &address);

}
