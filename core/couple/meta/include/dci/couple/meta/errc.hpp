#pragma once

#include "type.hpp"
#include "named.hpp"
#include "scoped.hpp"
#include "compound.hpp"
#include "errcValue.hpp"

namespace dci { namespace couple { namespace meta
{
    class Errc
        : public himpl::FaceLayout<impl::Errc, Type, Named, Scoped, Compound<ErrcValue>>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Errc() = delete;
        ~Errc() = delete;

        static const TypeConcrete _concrete = TypeConcrete::errc;

        const std::vector<const ErrcValue *> &values() const;

    };

}}}
