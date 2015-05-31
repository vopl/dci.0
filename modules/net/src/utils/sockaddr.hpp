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

    void fillSockaddr(sockaddr_in &inetAddress, const net::ip4::Address &address);
    void fillSockaddr(sockaddr_in6 &inetAddress, const net::ip6::Address &address);
    void fillSockaddr(sockaddr_un &inetAddress, const net::local::Address &address);

}
