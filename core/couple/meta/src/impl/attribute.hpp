#pragma once

#include "named.hpp"
#include "type.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Attribute
        : public Named
    {
    public:
        Attribute();
        ~Attribute();

        void setType(Type *v);

        const Type *type() const;

    private:
        Type *_type{nullptr};
    };

}}}}
