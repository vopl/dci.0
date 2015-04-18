#pragma once

#include "signed.hpp"
#include "container1.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Ptr
        : public Signed
        , public Container1
    {
    public:
        Ptr();
        ~Ptr();

        void makeSign();
    };

}}}}
