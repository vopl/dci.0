#pragma once

#include "type.hpp"
#include "named.hpp"
#include "scoped.hpp"
#include "signed.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Alias
        : public Type
        , public Named
        , public Scoped
        , public Signed
    {
    public:
        Alias();
        ~Alias();

        void setType(Type *v);
        void makeSign() override;

    private:
        Type *_type{nullptr};
    };

}}}}
