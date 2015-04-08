#pragma once
#include <dci/couple/runtime/iface.hpp>
#include <dci/couple/runtime/call/wire.hpp>
#include <dci/couple/runtime/idl.hpp>

#include "../../scope/talk/Peer.hpp"
#include "../../talk/Chat.fwd.hpp"

namespace wire
{
    namespace talk
    {
        struct Peer
            : public ::dci::couple::runtime::IfaceWire
        {
            ::dci::couple::runtime::call::Wire< ::dci::couple::runtime::idl::void_(::scope::talk::Peer::Id)> setId;
            ::dci::couple::runtime::call::Wire< ::dci::couple::runtime::idl::void_(::scope::talk::Peer::Status)> setStatus;
            ::dci::couple::runtime::call::Wire< ::dci::couple::runtime::idl::bool_(::talk::Chat)> joinChat;

            Peer();
        };
    }
}
