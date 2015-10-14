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
    template <class T, class RemoveCleaner>
    IntrusiveDlist<T, RemoveCleaner>::IntrusiveDlist()
        : _first(intrusiveDlistElementCast(&_last, (T*)nullptr), nullptr)
        , _last(nullptr, intrusiveDlistElementCast(&_first, (T*)nullptr))
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class RemoveCleaner>
    IntrusiveDlist<T, RemoveCleaner>::IntrusiveDlist(T *element)
        : _first(element, nullptr)
        , _last(nullptr, element)
    {
        auto idee = intrusiveDlistElementCast(element);
        idee->_prev = &_first;
        idee->_next = &_last;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class RemoveCleaner>
    IntrusiveDlist<T, RemoveCleaner>::IntrusiveDlist(RemoveCleaner &&removeCleaner)
        : _first(intrusiveDlistElementCast(&_last, (T*)nullptr), nullptr)
        , _last(nullptr, intrusiveDlistElementCast(&_first, (T*)nullptr))
        , RemoveCleanExecutor<RemoveCleaner>(std::forward<RemoveCleaner>(removeCleaner))
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class RemoveCleaner>
    IntrusiveDlist<T, RemoveCleaner>::IntrusiveDlist(T *element, RemoveCleaner &&removeCleaner)
        : _first(element, nullptr)
        , _last(nullptr, element)
        , RemoveCleanExecutor<RemoveCleaner>(std::forward<RemoveCleaner>(removeCleaner))
    {
        auto idee = intrusiveDlistElementCast(element);
        idee->_prev = &_first;
        idee->_next = &_last;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class RemoveCleaner>
    IntrusiveDlist<T, RemoveCleaner>::~IntrusiveDlist()
    {
        assert(!_first._prev && !_last._next);
        assert(intrusiveDlistElementCast(_first._next) == &_last);
        assert(&_first == intrusiveDlistElementCast(_last._prev));
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class RemoveCleaner>
    bool IntrusiveDlist<T, RemoveCleaner>::empty() const
    {
        return intrusiveDlistElementCast(_first._next) == &_last;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class RemoveCleaner>
    T *IntrusiveDlist<T, RemoveCleaner>::first() const
    {
        if(empty())
        {
            return nullptr;
        }

        return _first._next;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class RemoveCleaner>
    T *IntrusiveDlist<T, RemoveCleaner>::last() const
    {
        if(empty())
        {
            return nullptr;
        }

        return intrusiveDlistElementCast(_last._prev, (T*)nullptr);
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class RemoveCleaner>
    std::pair<T*, T*> IntrusiveDlist<T, RemoveCleaner>::range() const
    {
        T *first = _first._next;
        T *last = intrusiveDlistElementCast(const_cast<IntrusiveDlistElement<T> *>(&_last), (T*)nullptr);
        return std::make_pair(
                    first,
                    last);
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class RemoveCleaner>
    bool IntrusiveDlist<T, RemoveCleaner>::contain(T *element) const
    {
        IntrusiveDlistElement<T> *first = intrusiveDlistElementCast(element);
        //T *last = element;

        while(first->_prev) first = intrusiveDlistElementCast(first->_prev);
        //while(last->_next) last = last->_next;

        return &_first == first ;//&& _last == last;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class RemoveCleaner>
    void IntrusiveDlist<T, RemoveCleaner>::push(T *element)
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
    template <class T, class RemoveCleaner>
    void IntrusiveDlist<T, RemoveCleaner>::remove(T *element)
    {
        assert(contain(element));

        auto idee = intrusiveDlistElementCast(element);

        intrusiveDlistElementCast(idee->_prev)->_next = idee->_next;
        intrusiveDlistElementCast(idee->_next)->_prev = idee->_prev;

//        idee->_prev = nullptr;
//        idee->_next = nullptr;
        RemoveCleanExecutor<RemoveCleaner>::operator ()(element);
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class RemoveCleaner>
    void IntrusiveDlist<T, RemoveCleaner>::clear()
    {
        if(!RemoveCleanExecutor<RemoveCleaner>::_isNull)
        {
            T *element = _first._next;
            while(element != intrusiveDlistElementCast(&_last, (T*)nullptr))
            {
                T *next = element->_next;
                RemoveCleanExecutor<RemoveCleaner>::operator ()(element);

                element = next;
            }
        }

        _first._next = intrusiveDlistElementCast(&_last, (T*)nullptr);
        _last._prev = intrusiveDlistElementCast(&_first, (T*)nullptr);
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class RemoveCleaner>
    template <class F>
    void IntrusiveDlist<T, RemoveCleaner>::each(F &&f)
    {
        T *element = _first._next;
        while(element != intrusiveDlistElementCast(&_last, (T*)nullptr))
        {
            f(element);
            element = intrusiveDlistElementCast(element)->_next;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class RemoveCleaner>
    template <class F>
    void IntrusiveDlist<T, RemoveCleaner>::flush(F &&f)
    {
        T *element = _first._next;
        while(element != intrusiveDlistElementCast(&_last, (T*)nullptr))
        {
            f(element);
            T *next = element->_next;
            RemoveCleanExecutor<RemoveCleaner>::operator ()(element);
            element = next;
        }

        _first._next = intrusiveDlistElementCast(&_last, (T*)nullptr);
        _last._prev = intrusiveDlistElementCast(&_first, (T*)nullptr);
    }

}}
