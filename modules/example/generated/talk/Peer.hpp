#pragma once
#include "Peer.fwd.hpp"
#include "details/PeerScope.hpp"
#include "details/PeerState.hpp"

namespace talk
{
    template <class T>
    using ValuePorter = ::dci::couple::runtime::call::ValuePorter<T>;

    template <class... T>
    using Future = ::dci::async::Future<::dci::couple::runtime::call::Error, ValuePorter<T>...>;

    template <class F>
    using Signal = ::dci::couple::runtime::call::Signal<F>;

    struct PeerOpposite;

    struct Peer
        : public ::talk::details::PeerScope
        , public ::dci::couple::runtime::Iface
    {
        using ::talk::details::PeerScope::Id;
        using ::talk::details::PeerScope::Status;

        Peer();
        Peer(Peer &&from);
        Peer(::dci::couple::runtime::Iface &&from);
        Peer(PeerOpposite &);
        ~Peer();

        Peer &operator=(Peer &&from);
        Peer &operator=(::dci::couple::runtime::Iface &&from);



        ::talk::details::PeerState *state();


        Future<> setId(ValuePorter<Id> &&id);
        Future<> setStatus(ValuePorter<Status> &&id);

        Future<bool> joinChat(ValuePorter<Chat> &&chat);
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
        PeerOpposite(Peer &from);
        ~PeerOpposite();

        PeerOpposite &operator=(PeerOpposite &&from);
        PeerOpposite &operator=(::dci::couple::runtime::Iface &&from);



        ::talk::details::PeerState *state();


        Signal<void(Id)> &setId();
        Signal<void(Status)> &setStatus();

        Signal<bool(Chat)> &joinChat();
    };

}
