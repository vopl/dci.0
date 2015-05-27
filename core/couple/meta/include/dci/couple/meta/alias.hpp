#pragma once

#include "type.hpp"
#include "named.hpp"
#include "scoped.hpp"

namespace dci { namespace couple { namespace meta
{
    class APIDCI_COUPLE_META Alias
        : public himpl::FaceLayout<impl::Alias, Type, Named, Scoped>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Alias() = delete;
        ~Alias() = delete;

        static const TypeConcrete _concrete = TypeConcrete::alias;

        const Type *type() const ;

    };

}}}
