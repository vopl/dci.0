#pragma once

#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")
#include <dci/himpl/faceLayout.hpp>

#include <string>
#include <cstdint>

namespace dci { namespace couple { namespace meta
{
    class Sign
        : public himpl::FaceLayout<impl::Sign>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Sign() = delete;
        ~Sign() = delete;

        std::string string(std::size_t chars=64) const;
    };
}}}
