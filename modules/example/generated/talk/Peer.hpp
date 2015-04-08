#pragma once
#include "Peer.fwd.hpp"
#include "../scope/talk/Peer.hpp"
#include "../wire/talk/Peer.hpp"
#include <dci/couple/runtime/idl.hpp>








namespace talk
{
    struct Peer
        : public ::scope::talk::Peer
        , private ::dci::couple::runtime::Iface
    {
        using ::scope::talk::Peer::Id;
        using ::scope::talk::Peer::Status;

        Peer();
        Peer(Peer &&from);
        Peer(::dci::couple::runtime::Iface &&from);
        Peer(PeerOpposite &);
        ~Peer();

        Peer &operator=(Peer &&from);
        Peer &operator=(::dci::couple::runtime::Iface &&from);





        Future<> setId(Id &&id);
        Future<> setId(const Id &id);
        Future<> setId(ValuePorter<Id> &&id);

        Future<> setStatus(Status &&id);
        Future<> setStatus(const Status &id);
        Future<> setStatus(ValuePorter<Status> &&id);

        Future<bool> joinChat(Chat &&chat);
        Future<bool> joinChat(ValuePorter<Chat> &&chat);

    private:
        friend class ::talk::PeerOpposite;
        ::wire::talk::Peer *wire();
    };

}

namespace talk
{
    struct PeerOpposite
        : public ::scope::talk::Peer
        , private ::dci::couple::runtime::Iface
    {
        using ::scope::talk::Peer::Id;
        using ::scope::talk::Peer::Status;

        PeerOpposite();
        PeerOpposite(PeerOpposite &&from);
        PeerOpposite(::dci::couple::runtime::Iface &&from);
        PeerOpposite(::talk::Peer &from);
        ~PeerOpposite();

        PeerOpposite &operator=(PeerOpposite &&from);
        PeerOpposite &operator=(::dci::couple::runtime::Iface &&from);





        Signal<void(Id)> &setId();
        Signal<void(Status)> &setStatus();

        Signal<bool(Chat)> &joinChat();

    private:
        friend class ::talk::Peer;
        ::wire::talk::Peer *wire();
    };
}
