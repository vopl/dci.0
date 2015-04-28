#pragma once

#include <dci/couple/runtime/sign.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Signed
    {
    public:
        Signed();
        ~Signed();

        const runtime::Sign &sign() const;
        void setSign(const runtime::Sign &s);

        void makeSign() = delete;

    private:
        runtime::Sign _sign;
        int k;
    };

}}}}
