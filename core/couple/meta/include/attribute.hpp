#pragma once

#include "named.hpp"
#include "signed.hpp"

namespace dci { namespace couple { namespace meta
{
    class Attribute
        : public himpl::FaceLayout<impl::Attribute, Named, Signed>
    {
    public:
        const Type *type() const;

    };

}}}
