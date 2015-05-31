#include "sockaddr.hpp"

#include <cstring>

namespace utils
{

    void fillSockaddr(sockaddr_in &inetAddress, const net::ip4::Address &address)
    {
        inetAddress.sin_family = AF_INET;
        inetAddress.sin_port = htons(address.port);
        memcpy(&inetAddress.sin_addr.s_addr, address.octets.data(), sizeof(address.octets));
    }

    void fillSockaddr(sockaddr_in6 &inetAddress, const net::ip6::Address &address)
    {
        inetAddress.sin6_family = AF_INET6;
        inetAddress.sin6_port = htons(address.port);
        memcpy(inetAddress.sin6_addr.__in6_u.__u6_addr8, address.octets.data(), sizeof(address.octets));
    }

    void fillSockaddr(sockaddr_un &inetAddress, const net::local::Address &address)
    {
        inetAddress.sun_family = AF_LOCAL;
        strncpy (inetAddress.sun_path, address.key.data(), sizeof(inetAddress.sun_path));
        inetAddress.sun_path[sizeof(inetAddress.sun_path) - 1] = '\0';
    }
}
