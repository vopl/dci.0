#pragma once

#include "sign.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Scope;

    class Scoped
    {
    public:
        Scoped();
        ~Scoped();

        const Scope *scope() const;
        void setScope(Scope *scope);

        Sign scopesSign() const;

    private:
        Scope *_scope{nullptr};
    };

}}}}
