#pragma once

#include "named.hpp"
#include "signed.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class EnumValue
        : public Named
        , public Signed
    {
    public:
        EnumValue();
        ~EnumValue();

        void makeSign() override;
    };

}}}}
