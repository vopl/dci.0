#include "talk.hpp"

#include "talk/Peer.hpp"
#include "talk/Chat.hpp"


namespace wire
{
    namespace talk
    {
        Peer::Peer()
            : ::dci::couple::runtime::IfaceWire([](::dci::couple::runtime::IfaceWire *p){delete static_cast<Peer*>(p);})
        {
        }
    }
}

namespace talk
{

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Peer::Peer()
        : ::dci::couple::runtime::Iface(new ::wire::talk::Peer, true)
    {
    }

    Peer::Peer(Peer &&from)
        : ::dci::couple::runtime::Iface(std::forward< ::dci::couple::runtime::Iface>(from))
    {
    }

    Peer::Peer(::dci::couple::runtime::Iface &&from)
        : ::dci::couple::runtime::Iface(std::forward< ::dci::couple::runtime::Iface>(from))
    {
        assert(0);
    }

    Peer::Peer(::talk::PeerOpposite &from)
        : ::dci::couple::runtime::Iface(from.wire(), true)
    {
    }

    Peer::~Peer()
    {
    }

    Peer &Peer::operator=(Peer &&from)
    {
        ::dci::couple::runtime::Iface::operator=(std::forward< ::dci::couple::runtime::Iface>(from));
        return *this;
    }

    Peer &Peer::operator=(::dci::couple::runtime::Iface &&from)
    {
        (void)from;
        assert(0);
    }

    ::wire::talk::Peer *Peer::wire()
    {
        return static_cast< ::wire::talk::Peer *>(Iface::wire());
    }

    Peer::Future<> Peer::setId(Id &&id)
    {
        return wire()->setId(ValuePorter<Id>(std::forward<Id>(id)));
    }

    Peer::Future<> Peer::setId(const Id &id)
    {
        return wire()->setId(ValuePorter<Id>(id));
    }

    Peer::Future<> Peer::setStatus(Status &&status)
    {
        return wire()->setStatus(ValuePorter<Status>(std::forward<Status>(status)));
    }

    Peer::Future<> Peer::setStatus(const Status &status)
    {
        return wire()->setStatus(ValuePorter<Status>(status));
    }

    Peer::Future<bool> Peer::joinChat(Chat &&chat)
    {
        return wire()->joinChat(ValuePorter<Chat>(std::forward<Chat>(chat)));
    }


}


namespace talk
{


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    PeerOpposite::PeerOpposite()
        : ::dci::couple::runtime::Iface(new ::wire::talk::Peer, false)
    {
    }

    PeerOpposite::PeerOpposite(PeerOpposite &&from)
        : ::dci::couple::runtime::Iface(std::forward< ::dci::couple::runtime::Iface>(from))
    {
    }

    PeerOpposite::PeerOpposite(::dci::couple::runtime::Iface &&from)
        : ::dci::couple::runtime::Iface(std::forward< ::dci::couple::runtime::Iface>(from))
    {
        assert(0);
    }

    PeerOpposite::PeerOpposite(::talk::Peer &from)
        : ::dci::couple::runtime::Iface(from.wire(), false)
    {
    }

    PeerOpposite::~PeerOpposite()
    {
    }


    PeerOpposite &PeerOpposite::operator=(PeerOpposite &&from)
    {
        ::dci::couple::runtime::Iface::operator=(std::forward< ::dci::couple::runtime::Iface>(from));
        return *this;
    }

    PeerOpposite &PeerOpposite::operator=(::dci::couple::runtime::Iface &&from)
    {
        (void)from;
        assert(0);
    }




    ::wire::talk::Peer *PeerOpposite::wire()
    {
        return static_cast< ::wire::talk::Peer *>(Iface::wire());
    }



    ::dci::couple::runtime::idl::Signal<void(::scope::talk::Peer::Id)> &PeerOpposite::setId()
    {
        return wire()->setId;
    }

    ::dci::couple::runtime::idl::Signal<void(::scope::talk::Peer::Status)> &PeerOpposite::setStatus()
    {
        return wire()->setStatus;
    }

    ::dci::couple::runtime::idl::Signal<bool(::talk::Chat)> &PeerOpposite::joinChat()
    {
        return wire()->joinChat;
    }


}


