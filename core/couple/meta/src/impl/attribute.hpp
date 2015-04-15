#pragma once

#include "named.hpp"
#include "signed.hpp"
#include "type.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Attribute
        : public Named
        , public Signed
    {
    public:
        Attribute();
        ~Attribute();

        void setType(Type *v);
        const Type *type() const;
        void makeSign() override;

    private:
        Type *_type{nullptr};
    };

}}}}
