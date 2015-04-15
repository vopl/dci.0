#pragma once

#include <type.hpp>
#include "sign.hpp"


namespace dci { namespace couple { namespace meta { namespace impl
{
    class Type
    {

    protected:
        Type(TypeConcrete concrete);
        ~Type();

    public:
        TypeConcrete concrete() const;

        const Sign &concreteSign() const;

    private:
        TypeConcrete _concrete{TypeConcrete::null};

    };

}}}}
