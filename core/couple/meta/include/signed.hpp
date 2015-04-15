#pragma once

#include "sign.hpp"

namespace dci { namespace couple { namespace meta
{
    class Signed
        : public himpl::FaceLayout<impl::Signed>
    {
    public:
        const Sign &sign() const;

    };

}}}
