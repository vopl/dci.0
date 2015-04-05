#pragma once
#include <dci/couple/runtime/iface.hpp>
#include <dci/couple/runtime/call/wire.hpp>
#include <dci/couple/runtime/idl.hpp>

#include "PeerScope.hpp"
#include "../Chat.fwd.hpp"

namespace talk
{
    namespace details
    {
        struct PeerState
            : public ::dci::couple::runtime::IfaceState
        {
            ::dci::couple::runtime::call::Wire<::dci::couple::runtime::idl::void_(::talk::details::PeerScope::Id)> setId;
            ::dci::couple::runtime::call::Wire<::dci::couple::runtime::idl::void_(::talk::details::PeerScope::Status)> setStatus;
            ::dci::couple::runtime::call::Wire<::dci::couple::runtime::idl::bool_(::talk::Chat)> joinChat;

            PeerState();
        };
    }
}
