#pragma once

#include "sign.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Signed
    {
    public:
        Signed();
        virtual ~Signed();

        const Sign &sign() const;
        void setSign(const Sign &s);

        virtual void makeSign() = 0;

    private:
        Sign _sign;
    };

}}}}
