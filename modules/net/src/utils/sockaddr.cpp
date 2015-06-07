#include "sockaddr.hpp"

#include <cstring>

namespace utils
{

    void fillSockaddr(const net::ip4::Address &address, sockaddr_in &inetAddress)
    {
        inetAddress.sin_family = AF_INET;
        inetAddress.sin_port = htons(address.port);
        memcpy(&inetAddress.sin_addr.s_addr, address.octets.data(), sizeof(address.octets));
    }

    void fillSockaddr(const net::ip6::Address &address, sockaddr_in6 &inetAddress)
    {
        inetAddress.sin6_family = AF_INET6;
        inetAddress.sin6_port = htons(address.port);
        memcpy(inetAddress.sin6_addr.__in6_u.__u6_addr8, address.octets.data(), sizeof(address.octets));
    }

    void fillSockaddr(const net::local::Address &address, sockaddr_un &inetAddress)
    {
        inetAddress.sun_family = AF_LOCAL;
        strncpy (inetAddress.sun_path, address.key.data(), sizeof(inetAddress.sun_path));
        inetAddress.sun_path[sizeof(inetAddress.sun_path) - 1] = '\0';
    }





    void fillSockaddr(const sockaddr_in &inetAddress, net::ip4::Address &address)
    {
        address.port = ntohs(inetAddress.sin_port);
        memcpy(address.octets.data(), &inetAddress.sin_addr.s_addr, sizeof(address.octets));
    }

    void fillSockaddr(const sockaddr_in6 &inetAddress, net::ip6::Address &address)
    {
        address.port = ntohs(inetAddress.sin6_port);
        memcpy(address.octets.data(), inetAddress.sin6_addr.__in6_u.__u6_addr8, sizeof(address.octets));
    }

    void fillSockaddr(const sockaddr_un &inetAddress, net::local::Address &address)
    {
        address.key.resize(strlen(inetAddress.sun_path));
        strncpy (&address.key[0], inetAddress.sun_path, sizeof(inetAddress.sun_path));
    }
}
