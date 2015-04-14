#pragma once

#include <dci/himpl/implLayout.hpp>

#include <vector>

namespace dci { namespace couple { namespace meta { namespace impl
{
    template <class E>
    class Compound
        : public himpl::ImplLayout<Compound<E>>
    {
    public:
        Compound();
        ~Compound();

        void add(E *element);

        const std::vector<E *> &elements() const;

    private:
        std::vector<E *> _elements;
    };



    template <class E>
    Compound<E>::Compound()
    {

    }

    template <class E>
    Compound<E>::~Compound()
    {

    }

    template <class E>
    void Compound<E>::add(E *element)
    {
        _elements.push_back(element);
    }

    template <class E>
    const std::vector<E *> &Compound<E>::elements() const
    {
        return _elements;
    }

}}}}
