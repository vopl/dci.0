#pragma once

#include "type.hpp"
#include "named.hpp"
#include "scoped.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Alias
        : public Type
        , public Named
        , public Scoped
    {
    public:
        Alias();
        ~Alias();

        void setType(Type *v);
        const Type *type() const;
        void makeSign();

    private:
        Type *_type{nullptr};
    };

}}}}
