#pragma once

#include "named.hpp"
#include "signed.hpp"
#include "compound.hpp"
#include "type.hpp"

namespace dci { namespace couple { namespace meta
{
    enum class CallDirection
    {
        in,
        out
    };

    class Method
        : public himpl::FaceLayout<impl::Method, Named, Signed, Compound<Attribute>>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Method() = delete;
        ~Method() = delete;

        CallDirection direction() const;
        bool nowait() const;
        const Type *resultType() const;
        const std::vector<const Attribute *> &attributes() const;
    };

}}}
