#pragma once
#include <dci/couple/runtime/idl.hpp>

namespace talk
{
    namespace details
    {
        struct PeerScope
        {
            using Id = ::dci::couple::runtime::idl::array< ::dci::couple::runtime::idl::int8, 32>;

            enum class Status
            {
                available=0,
                busy=1,
            };
        };
    }
}
