#pragma once

#include <dci/himpl/sizeProvider.hpp>
#include SIZEPROVIDERFILE("sizeProvider.hpp")
#include <dci/himpl/faceLayout.hpp>

namespace dci { namespace couple { namespace meta
{
    class Scoped
        : public himpl::FaceLayout<impl::Scoped>
    {
    public:
        const Scope *scope() const;

    };

}}}
