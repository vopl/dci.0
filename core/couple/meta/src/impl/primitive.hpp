#pragma once

#include "type.hpp"
#include "signed.hpp"
#include <dci/couple/meta/primitive.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Primitive
        : public Type
        , public Signed
    {
    public:
        Primitive();
        ~Primitive();

        void setKind(PrimitiveKind kind);
        PrimitiveKind kind() const;
        void makeSign();

    private:
        PrimitiveKind _kind = PrimitiveKind::void_;
    };

}}}}
