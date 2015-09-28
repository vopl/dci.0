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
            using ChannelHandlerSkel = generated::handlers::net::ip4::datagram::Channel<H>;
        };

        struct stream
        {
            using Server = ip4::stream::Server;
            using Client = ip4::stream::Client;
            using Channel = ip4::stream::Channel;

            template <class H>
            using ServerHandlerSkel = generated::handlers::net::ip4::stream::Server<H>;

            template <class H>
            using ClientHandlerSkel = generated::handlers::net::ip4::stream::Client<H>;

            template <class H>
            using ChannelHandlerSkel = generated::handlers::net::ip4::stream::Channel<H>;
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
            using ChannelHandlerSkel = generated::handlers::net::ip6::datagram::Channel<H>;
        };

        struct stream
        {
            using Server = ip6::stream::Server;
            using Client = ip6::stream::Client;
            using Channel = ip6::stream::Channel;

            template <class H>
            using ServerHandlerSkel = generated::handlers::net::ip6::stream::Server<H>;

            template <class H>
            using ClientHandlerSkel = generated::handlers::net::ip6::stream::Client<H>;

            template <class H>
            using ChannelHandlerSkel = generated::handlers::net::ip6::stream::Channel<H>;
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
            using ServerHandlerSkel = generated::handlers::net::local::stream::Server<H>;

            template <class H>
            using ClientHandlerSkel = generated::handlers::net::local::stream::Client<H>;

            template <class H>
            using ChannelHandlerSkel = generated::handlers::net::local::stream::Channel<H>;
        };
    };
}
