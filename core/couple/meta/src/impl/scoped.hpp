#pragma once

#include "signed.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Scope;

    class Scoped
        : public Signed
    {
    public:
        Scoped();
        ~Scoped();

        const Scope *scope() const;
        void setScope(Scope *scope);

        void makeSign();

    private:
        Scope *_scope{nullptr};
    };

}}}}
