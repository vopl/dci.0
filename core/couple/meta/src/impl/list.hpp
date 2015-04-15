#pragma once

#include "signed.hpp"
#include "container1.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class List
        : public Signed
        , public Container1
    {
    public:
        List();
        ~List();

        void makeSign() override;
    };

}}}}
