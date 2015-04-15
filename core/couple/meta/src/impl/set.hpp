#pragma once

#include "signed.hpp"
#include "container1.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Set
        : public Signed
        , public Container1
    {
    public:
        Set();
        ~Set();

        void makeSign() override;
    };

}}}}
