#pragma once

#include <type_traits>
#include <utility>

namespace dci { namespace mm
{
    template <class T>
    struct IntrusiveDlistElement
    {
        IntrusiveDlistElement(T *next=nullptr, T *prev=nullptr);

        T *_next;
        T *_prev;
    };

    template <class T> std::enable_if_t<std::is_base_of<IntrusiveDlistElement<T> *, T *>::value, IntrusiveDlistElement<T> *> intrusiveDlistElementCast(T *e);
    template <class T> std::enable_if_t<std::is_base_of<IntrusiveDlistElement<T> *, T *>::value, T *> intrusiveDlistElementCast(IntrusiveDlistElement<T> *e, T *stubForADL);

    template <class T>
    class IntrusiveDlist
    {
    public:
        IntrusiveDlist();
        IntrusiveDlist(T *element);
        ~IntrusiveDlist();

    public:
        bool empty() const;
        T *first() const;
        T *last() const;
        std::pair<T*, T*> range() const;
        bool contain(T *element) const;
        void push(T *element);
        void remove(T *element);

    private:
        IntrusiveDlistElement<T> _first;
        IntrusiveDlistElement<T> _last;
    };
}}

#include "intrusiveDlist.ipp"
