#include "talk.hpp"

namespace dci { namespace couple { namespace runtime { namespace generated
{

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <int i>
    wire<i>::talk::Peer::Peer()
        : IfaceWire([](IfaceWire *p){delete static_cast<Peer*>(p);})
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <int i>
    body<i>::talk::Peer::Peer()
        : Iface(new typename wire<i>::talk::Peer, true)
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <int i>
    body<i>::talk::Peer::Peer(Peer &&from)
        : Iface(std::forward< Iface>(from))
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <int i>
    body<i>::talk::Peer::Peer(Iface &&from)
        : Iface(std::forward< Iface>(from))
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <int i>
    body<i>::talk::Peer::Peer(typename body<i>::talk::PeerOpposite &from)
        : Iface(from.wire(), true)
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <int i>
    body<i>::talk::Peer::~Peer()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <int i>
    typename body<i>::talk::Peer &body<i>::talk::Peer::operator=(Peer &&from)
    {
        Iface::operator=(std::forward< Iface>(from));
        return *this;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <int i>
    typename body<i>::talk::Peer &body<i>::talk::Peer::operator=(Iface &&from)
    {
        (void)from;
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <int i>
    typename wire<i>::talk::Peer *body<i>::talk::Peer::wire()
    {
        return static_cast< typename wire<i>::talk::Peer *>(Iface::wire());
    }




    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <int i>
    idl::Future<> body<i>::talk::Peer::setId(Id &&id)
    {
        return wire()->setId(idl::ValuePorter<Id>(std::forward<Id>(id)));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <int i>
    idl::Future<> body<i>::talk::Peer::setId(const Id &id)
    {
        return wire()->setId(idl::ValuePorter<Id>(id));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <int i>
    idl::Future<> body<i>::talk::Peer::setId(idl::ValuePorter<Id> &&id)
    {
        return wire()->setId(std::forward< idl::ValuePorter<Id>>(id));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <int i>
    idl::Future<> body<i>::talk::Peer::setStatus(Status &&status)
    {
        return wire()->setStatus(idl::ValuePorter<Status>(std::forward<Status>(status)));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <int i>
    idl::Future<> body<i>::talk::Peer::setStatus(const Status &status)
    {
        return wire()->setStatus(idl::ValuePorter<Status>(status));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <int i>
    idl::Future<> body<i>::talk::Peer::setStatus(idl::ValuePorter<Status> &&status)
    {
        return wire()->setStatus(std::forward< idl::ValuePorter<Status>>(status));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <int i>
    idl::Future<bool> body<i>::talk::Peer::joinChat(typename body<i>::talk::Chat &&chat)
    {
        return wire()->joinChat(idl::ValuePorter<Chat>(std::forward<Chat>(chat)));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <int i>
    idl::Future<bool> body<i>::talk::Peer::joinChat(idl::ValuePorter<typename body<i>::talk::Chat> &&chat)
    {
        return wire()->joinChat(std::forward< idl::ValuePorter<typename body<i>::talk::Chat>>(chat));
    }








    template <int i>
    body<i>::talk::PeerOpposite::PeerOpposite()
        : Iface(new typename wire<i>::talk::Peer, false)
    {
    }

    template <int i>
    body<i>::talk::PeerOpposite::PeerOpposite(PeerOpposite &&from)
        : Iface(std::forward< Iface>(from))
    {
    }

    template <int i>
    body<i>::talk::PeerOpposite::PeerOpposite(Iface &&from)
        : Iface(std::forward< Iface>(from))
    {
        assert(0);
    }

    template <int i>
    body<i>::talk::PeerOpposite::PeerOpposite(typename body<i>::talk::Peer &from)
        : Iface(from.wire(), false)
    {
    }

    template <int i>
    body<i>::talk::PeerOpposite::~PeerOpposite()
    {
    }

    template <int i>
    typename body<i>::talk::PeerOpposite &body<i>::talk::PeerOpposite::operator=(PeerOpposite &&from)
    {
        Iface::operator=(std::forward< Iface>(from));
        return *this;
    }

    template <int i>
    typename body<i>::talk::PeerOpposite &body<i>::talk::PeerOpposite::operator=(Iface &&from)
    {
        (void)from;
        assert(0);
    }

    template <int i>
    typename wire<i>::talk::Peer *body<i>::talk::PeerOpposite::wire()
    {
        return static_cast< typename wire<i>::talk::Peer *>(Iface::wire());
    }

    template <int i>
    idl::Signal<void(typename body<i>::talk::PeerOpposite::Id)> &body<i>::talk::PeerOpposite::setId()
    {
        return wire()->setId;
    }

    template <int i>
    idl::Signal<void(typename body<i>::talk::PeerOpposite::Status)> &body<i>::talk::PeerOpposite::setStatus()
    {
        return wire()->setStatus;
    }

    template <int i>
    idl::Signal<bool(typename body<i>::talk::Chat)> &body<i>::talk::PeerOpposite::joinChat()
    {
        return wire()->joinChat;
    }












    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template class wire<0>;
    template class body<0>;
}}}}
