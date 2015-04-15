#pragma once

#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")
#include <dci/himpl/faceLayout.hpp>

namespace dci { namespace couple { namespace runtime
{
    namespace impl
    {
        class Hub;
    }

    class Hub
        : public himpl::FaceLayout<impl::Hub>
    {
    };

}}}
