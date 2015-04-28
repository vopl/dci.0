#pragma once

#include <type.hpp>
#include <dci/couple/runtime/sign.hpp>


namespace dci { namespace couple { namespace meta { namespace impl
{
    class Type
    {

    protected:
        Type(TypeConcrete concrete);
        ~Type();

    public:
        TypeConcrete concrete() const;

        const runtime::Sign &concreteSign() const;

    private:
        TypeConcrete _concrete{TypeConcrete::null};

    };

}}}}
