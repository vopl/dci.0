#pragma once

#include "sign.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Signed
    {
    public:
        Signed();
        ~Signed();

        const Sign &sign() const;
        void setSign(const Sign &s);

        void makeSign() = delete;

    private:
        Sign _sign;
    };

}}}}
