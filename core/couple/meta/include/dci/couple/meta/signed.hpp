#pragma once

#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")
#include <dci/himpl/faceLayout.hpp>

#include <dci/couple/runtime/sign.hpp>

namespace dci { namespace couple { namespace meta
{
    class Signed
        : public himpl::FaceLayout<impl::Signed>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Signed();
        ~Signed();

        const runtime::Sign &sign() const;

    };

}}}
