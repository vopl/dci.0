#pragma once

#include "net.hpp"
#include "netHandlerSkel.hpp"

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace net;

    template <class Address>
    struct ScopeSelector;

    template <>
    struct ScopeSelector<net::ip4::Address>
    {
        using Address = ip4::Address;

        struct datagram
        {
            using Channel = ip4::datagram::Channel;

            template <class H>
            using HostHandlerSkel = hs::net::ip4::datagram::Host<H>;

            template <class H>
            using ChannelHandlerSkel = hs::net::ip4::datagram::Channel<H>;
        };

        struct stream
        {
            using Server = ip4::stream::Server;
            using Client = ip4::stream::Client;
            using Channel = ip4::stream::Channel;

            template <class H>
            using HostHandlerSkel = hs::net::ip4::stream::Host<H>;

            template <class H>
            using ServerHandlerSkel = hs::net::ip4::stream::Server<H>;

            template <class H>
            using ClientHandlerSkel = hs::net::ip4::stream::Client<H>;

            template <class H>
            using ChannelHandlerSkel = hs::net::ip4::stream::Channel<H>;
        };
    };

    template <>
    struct ScopeSelector<ip6::Address>
    {
        using Address = ip6::Address;

        struct datagram
        {
            using Channel = ip6::datagram::Channel;

            template <class H>
            using HostHandlerSkel = hs::net::ip6::datagram::Host<H>;

            template <class H>
            using ChannelHandlerSkel = hs::net::ip6::datagram::Channel<H>;
        };

        struct stream
        {
            using Server = ip6::stream::Server;
            using Client = ip6::stream::Client;
            using Channel = ip6::stream::Channel;

            template <class H>
            using HostHandlerSkel = hs::net::ip6::stream::Host<H>;

            template <class H>
            using ServerHandlerSkel = hs::net::ip6::stream::Server<H>;

            template <class H>
            using ClientHandlerSkel = hs::net::ip6::stream::Client<H>;

            template <class H>
            using ChannelHandlerSkel = hs::net::ip6::stream::Channel<H>;
        };
    };

    template <>
    struct ScopeSelector<local::Address>
    {
        using Address = local::Address;

        struct stream
        {
            using Server = local::stream::Server;
            using Client = local::stream::Client;
            using Channel = local::stream::Channel;

            template <class H>
            using HostHandlerSkel = hs::net::local::stream::Host<H>;

            template <class H>
            using ServerHandlerSkel = hs::net::local::stream::Server<H>;

            template <class H>
            using ClientHandlerSkel = hs::net::local::stream::Client<H>;

            template <class H>
            using ChannelHandlerSkel = hs::net::local::stream::Channel<H>;
        };
    };
}
