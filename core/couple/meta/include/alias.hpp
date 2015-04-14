#pragma once

#include "type.hpp"
#include "named.hpp"
#include "scoped.hpp"

namespace dci { namespace couple { namespace meta
{
    class Alias
        : public himpl::FaceLayout<impl::Alias, Type, Named, Scoped>
    {
    public:
        static const TypeConcrete _concrete = TypeConcrete::alias;

        Type *target();

        bool strong();

    };

}}}
