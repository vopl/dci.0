#pragma once

#include "signed.hpp"
#include "container2.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Map
        : public Signed
        , public Container2
    {
    public:
        Map();
        ~Map();

        void makeSign();
    };

}}}}
