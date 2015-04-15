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
        Sign();
        ~Sign();

        std::string string(std::size_t chars=64) const;
    };
}}}
