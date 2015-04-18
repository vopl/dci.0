#pragma once

#include <vector>

namespace dci { namespace couple { namespace meta { namespace impl
{
    template <class B>
    class Inheritable
    {
    public:
        Inheritable();
        ~Inheritable();

        void addBase(B *);
        const std::vector<const B*> &bases() const;

    private:
        std::vector<const B*> _bases;
    };



    template <class B>
    Inheritable<B>::Inheritable()
    {
    }

    template <class B>
    Inheritable<B>::~Inheritable()
    {
    }

    template <class B>
    void Inheritable<B>::addBase(B *v)
    {
        _bases.push_back(v);
    }

    template <class B>
    const std::vector<const B*> &Inheritable<B>::bases() const
    {
        return _bases;
    }


}}}}
