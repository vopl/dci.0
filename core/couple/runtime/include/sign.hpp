#pragma once

#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")
#include <dci/himpl/faceLayout.hpp>

#include <string>
#include <cstdint>

namespace dci { namespace couple { namespace runtime
{
    namespace impl
    {
        class Sign;
    }

    class Sign
        : public himpl::FaceLayout<impl::Sign>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Sign();
        Sign(const Sign &from);
        ~Sign();

        Sign &operator=(const Sign &from);

        std::string toHex(std::size_t chars=64) const;
        bool fromHex(const std::string &txt);
    };
}}}
