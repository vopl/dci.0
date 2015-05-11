#pragma once

#include "named.hpp"
#include "signed.hpp"
#include "compound.hpp"

namespace dci { namespace couple { namespace meta
{
    enum class CallDirection
    {
        in,
        out
    };

    class Method
        : public himpl::FaceLayout<impl::Method, Named, Signed, Compound<Attribute>, Compound<Type>>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Method() = delete;
        ~Method() = delete;

        CallDirection direction() const;
        const std::vector<const Attribute *> &query() const;
        const std::vector<const Type *> &reply() const;

        bool noreply() const;
    };

}}}
