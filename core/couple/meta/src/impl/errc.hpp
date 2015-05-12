#pragma once

#include "type.hpp"
#include "named.hpp"
#include "scoped.hpp"
#include "compound.hpp"
#include "errcValue.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Errc
        : public Type
        , public Named
        , public Scoped
        , public Compound<ErrcValue>
    {
    public:
        Errc();
        ~Errc();

        using Compound<ErrcValue>::add;
        void makeSign();
    };

}}}}
