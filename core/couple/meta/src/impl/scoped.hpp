#pragma once

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

    private:
        Scope *_scope{nullptr};
    };

}}}}
