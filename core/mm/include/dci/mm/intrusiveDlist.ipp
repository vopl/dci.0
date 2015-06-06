#pragma once

#include "intrusiveDlist.hpp"
#include <cassert>

namespace dci { namespace mm
{
    ////////////////////////////////////////////////////////////////////////////////
    template <class T>
    inline IntrusiveDlistElement<T>::IntrusiveDlistElement(T *next, T *prev)
        : _next(next)
        , _prev(prev)
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T> std::enable_if_t<std::is_base_of<IntrusiveDlistElement<T>, T>::value, IntrusiveDlistElement<T> *> intrusiveDlistElementCast(T *e)
    {
        return static_cast<IntrusiveDlistElement<T> *>(e);
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T> std::enable_if_t<std::is_base_of<IntrusiveDlistElement<T>, T>::value, T *> intrusiveDlistElementCast(IntrusiveDlistElement<T> *e, T *stubForADL)
    {
        (void)stubForADL;
        return static_cast<T *>(e);
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T>
    IntrusiveDlist<T>::IntrusiveDlist()
        : _first(intrusiveDlistElementCast(&_last, (T*)nullptr), nullptr)
        , _last(nullptr, intrusiveDlistElementCast(&_first, (T*)nullptr))
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T>
    IntrusiveDlist<T>::IntrusiveDlist(T *element)
        : _first(element, nullptr)
        , _last(nullptr, element)
    {
        auto idee = intrusiveDlistElementCast(element);
        idee->_prev = &_first;
        idee->_next = &_last;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T>
    IntrusiveDlist<T>::~IntrusiveDlist()
    {
        assert(!_first._prev && !_last._next);
        assert(intrusiveDlistElementCast(_first._next) == &_last);
        assert(&_first == intrusiveDlistElementCast(_last._prev));
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T>
    bool IntrusiveDlist<T>::empty() const
    {
        return intrusiveDlistElementCast(_first._next) == &_last;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T>
    T *IntrusiveDlist<T>::first() const
    {
        if(empty())
        {
            return nullptr;
        }

        return _first._next;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T>
    T *IntrusiveDlist<T>::last() const
    {
        if(empty())
        {
            return nullptr;
        }

        return intrusiveDlistElementCast(_last._prev, (T*)nullptr);
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T>
    std::pair<T*, T*> IntrusiveDlist<T>::range() const
    {
        IntrusiveDlistElement<T> *first = _first._next;
        IntrusiveDlistElement<T> *last = const_cast<IntrusiveDlistElement<T> *>(&_last);
        return std::make_pair(
                    intrusiveDlistElementCast(first, (T*)nullptr),
                    intrusiveDlistElementCast(last, (T*)nullptr));
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T>
    bool IntrusiveDlist<T>::contain(T *element) const
    {
        IntrusiveDlistElement<T> *first = intrusiveDlistElementCast(element);
        //T *last = element;

        while(first->_prev) first = intrusiveDlistElementCast(first->_prev);
        //while(last->_next) last = last->_next;

        return &_first == first ;//&& _last == last;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T>
    void IntrusiveDlist<T>::push(T *element)
    {
        auto idee = intrusiveDlistElementCast(element);

//        assert(!idee->_next);
//        assert(!idee->_prev);

        idee->_next = intrusiveDlistElementCast(&_last, (T*)nullptr);
        idee->_prev = _last._prev;
        intrusiveDlistElementCast(_last._prev)->_next = element;
        _last._prev = element;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T>
    void IntrusiveDlist<T>::remove(T *element)
    {
        assert(contain(element));

        auto idee = intrusiveDlistElementCast(element);

        intrusiveDlistElementCast(idee->_prev)->_next = idee->_next;
        intrusiveDlistElementCast(idee->_next)->_prev = idee->_prev;

//        idee->_prev = nullptr;
//        idee->_next = nullptr;
    }
}}
