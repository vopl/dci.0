#pragma once
#include "Peer.fwd.hpp"
#include "details/PeerScope.hpp"
#include "details/PeerState.hpp"

namespace talk
{
    struct Peer
        : public ::talk::details::PeerScope
        , public ::dci::couple::runtime::Iface
    {
        using ::talk::details::PeerScope::Id;
        using ::talk::details::PeerScope::Status;

        Peer();
        Peer(Peer &&from);
        Peer(::dci::couple::runtime::Iface &&from);
        ~Peer();

        Peer &operator=(Peer &&from);
        Peer &operator=(::dci::couple::runtime::Iface &&from);



        ::talk::details::PeerState *state();

        ::dci::async::Future<::dci::couple::runtime::call::Error> setId(const Id &id);
        ::dci::async::Future<::dci::couple::runtime::call::Error> setStatus(const Status &id);

        ::dci::async::Future<::dci::couple::runtime::call::Error, bool> joinChat(Chat &&chat);
    };

}
