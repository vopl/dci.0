#pragma once

#include <type.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Type
    {

    protected:
        Type(TypeConcrete concrete);

    public:
        TypeConcrete concrete() const;

    private:
        TypeConcrete _concrete{TypeConcrete::null};

    };

}}}}
