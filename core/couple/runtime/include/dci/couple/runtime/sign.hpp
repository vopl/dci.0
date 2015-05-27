#pragma once

#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")
#include <dci/himpl/faceLayout.hpp>

#include "api.hpp"
#include <string>
#include <cstdint>

namespace dci { namespace couple { namespace runtime
{
    namespace impl
    {
        class Sign;
    }

    class APIDCI_COUPLE_RUNTIME Sign
        : public himpl::FaceLayout<impl::Sign>
    {
    public:
        Sign();
        Sign(const std::uint8_t (&data)[16]);
        Sign(const Sign &from);
        Sign(Sign &&from);
        ~Sign();

        Sign &operator=(const Sign &from);
        Sign &operator=(Sign &&from);

        std::string toHex(std::size_t chars=32) const;
        bool fromHex(const std::string &txt);
        void fromRnd();

        bool operator<(const Sign &with) const;
        bool operator>(const Sign &with) const;
        bool operator<=(const Sign &with) const;
        bool operator>=(const Sign &with) const;
        bool operator==(const Sign &with) const;
        bool operator!=(const Sign &with) const;
    };
}}}
