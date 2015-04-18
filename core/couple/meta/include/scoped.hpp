#pragma once

#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")
#include <dci/himpl/faceLayout.hpp>

#include "signed.hpp"

namespace dci { namespace couple { namespace meta
{
    class Scoped
        : public himpl::FaceLayout<impl::Scoped, Signed>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Scoped() = delete;
        ~Scoped() = delete;

        const Scope *scope() const;

    };

}}}
