#pragma once

#include "sign.hpp"

namespace dci { namespace couple { namespace meta
{
    class Signed
        : public himpl::FaceLayout<impl::Signed>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Signed();
        ~Signed();

        const Sign &sign() const;

    };

}}}
