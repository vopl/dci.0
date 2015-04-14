#pragma once

#include "type.hpp"
#include <primitive.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Primitive
        : public Type
    {
    public:
        Primitive();
        ~Primitive();

        void setKind(PrimitiveKind kind);

        PrimitiveKind kind() const;

    private:
        PrimitiveKind _kind = PrimitiveKind::void_;
    };

}}}}
