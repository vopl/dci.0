#pragma once
#include "Peer.fwd.hpp"
#include "details/PeerScope.hpp"
#include "details/PeerState.hpp"

namespace talk
{
    template <class T>
    using Value = ::dci::couple::runtime::call::Value<T>;

    template <class... T>
    using Future = ::dci::async::Future<::dci::couple::runtime::call::Error, Value<T>...>;

    template <class F>
    using Signal = ::dci::couple::runtime::call::Signal<F>;

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


        Future<> setId(Value<Id> &&id);
        Future<> setStatus(Value<Status> &&id);

        Future<bool> joinChat(Value<Chat> &&chat);
    };




    struct PeerOpposite
        : public ::talk::details::PeerScope
        , public ::dci::couple::runtime::Iface
    {
        using ::talk::details::PeerScope::Id;
        using ::talk::details::PeerScope::Status;

        PeerOpposite();
        PeerOpposite(PeerOpposite &&from);
        PeerOpposite(::dci::couple::runtime::Iface &&from);
        ~PeerOpposite();

        PeerOpposite &operator=(PeerOpposite &&from);
        PeerOpposite &operator=(::dci::couple::runtime::Iface &&from);



        ::talk::details::PeerState *state();


        Signal<void(Id)> &setId();
        Signal<void(Status)> &setStatus();

        Signal<bool(Chat)> &joinChat();
    };

}
