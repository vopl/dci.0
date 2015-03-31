#pragma once
#include "Peer.fwd.hpp"
#include "details/PeerScope.hpp"
#include "details/PeerState.hpp"

namespace talk
{
    struct Peer
        : public ::talk::details::PeerScope
    {
        using ::talk::details::PeerScope::Id;
        using ::talk::details::PeerScope::Status;

    };
}
