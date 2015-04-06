#include "talk.idl.hpp"

#include "talk/Peer.hpp"
#include "talk/Chat.hpp"

namespace talk
{

    namespace details
    {
        PeerState::PeerState()
            : ::dci::couple::runtime::IfaceState([](::dci::couple::runtime::IfaceState *p){delete static_cast<PeerState*>(p);})
        {
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Peer::Peer()
        : ::dci::couple::runtime::Iface(new details::PeerState, true)
    {
    }

    Peer::Peer(Peer &&from)
        : ::dci::couple::runtime::Iface(std::forward<::dci::couple::runtime::Iface>(from))
    {
    }

    Peer::Peer(::dci::couple::runtime::Iface &&from)
        : ::dci::couple::runtime::Iface(std::forward<::dci::couple::runtime::Iface>(from))
    {
        assert(0);
    }

    Peer::Peer(PeerOpposite &from)
        : ::dci::couple::runtime::Iface(from.state(), true)
    {
    }

    Peer::~Peer()
    {
    }

    Peer &Peer::operator=(Peer &&from)
    {
        ::dci::couple::runtime::Iface::operator=(std::forward<::dci::couple::runtime::Iface>(from));
        return *this;
    }

    Peer &Peer::operator=(::dci::couple::runtime::Iface &&from)
    {
        (void)from;
        assert(0);
    }

    ::talk::details::PeerState *Peer::state()
    {
        return static_cast<::talk::details::PeerState *>(Iface::state());
    }

    Future<> Peer::setId(ValuePorter<Id> &&id)
    {
        return state()->setId(std::forward<ValuePorter<Id>>(id));
    }

    Future<> Peer::setStatus(ValuePorter<Status> &&status)
    {
        return state()->setStatus(std::forward<ValuePorter<Status>>(status));
    }

    Future<bool> Peer::joinChat(ValuePorter<Chat> &&chat)
    {
        return state()->joinChat(std::forward<ValuePorter<Chat>>(chat));
    }







    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    PeerOpposite::PeerOpposite()
        : ::dci::couple::runtime::Iface(new details::PeerState, false)
    {
    }

    PeerOpposite::PeerOpposite(PeerOpposite &&from)
        : ::dci::couple::runtime::Iface(std::forward<::dci::couple::runtime::Iface>(from))
    {
    }

    PeerOpposite::PeerOpposite(::dci::couple::runtime::Iface &&from)
        : ::dci::couple::runtime::Iface(std::forward<::dci::couple::runtime::Iface>(from))
    {
        assert(0);
    }

    PeerOpposite::PeerOpposite(Peer &from)
        : ::dci::couple::runtime::Iface(from.state(), false)
    {
    }

    PeerOpposite::~PeerOpposite()
    {
    }


    PeerOpposite &PeerOpposite::operator=(PeerOpposite &&from)
    {
        ::dci::couple::runtime::Iface::operator=(std::forward<::dci::couple::runtime::Iface>(from));
        return *this;
    }

    PeerOpposite &PeerOpposite::operator=(::dci::couple::runtime::Iface &&from)
    {
        (void)from;
        assert(0);
    }




    ::talk::details::PeerState *PeerOpposite::state()
    {
        return static_cast<::talk::details::PeerState *>(Iface::state());
    }



    Signal<void(::talk::details::PeerScope::Id)> &PeerOpposite::setId()
    {
        return state()->setId;
    }

    Signal<void(::talk::details::PeerScope::Status)> &PeerOpposite::setStatus()
    {
        return state()->setStatus;
    }

    Signal<bool(Chat)> &PeerOpposite::joinChat()
    {
        return state()->joinChat;
    }


}

