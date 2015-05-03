#pragma once

#include <type_traits>
#include <utility>
#include <cassert>

namespace dci { namespace couple { namespace runtime { namespace call
{
    template <class T>
    class ValuePorter
    {
        ValuePorter() = delete;
        ValuePorter(const ValuePorter &v) = delete;
        ValuePorter &operator=(const ValuePorter &) = delete;
        ValuePorter &operator=(ValuePorter &&) = delete;

    public:
        ValuePorter(ValuePorter &&);
        ValuePorter(const T &);

        ValuePorter(T &&v);
        ~ValuePorter();

        operator const T&() const;
        operator T&&();

    private:
        T *_ptr;
        bool _isConst;
        bool _isOwn;
    };




    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    ValuePorter<T>::ValuePorter(ValuePorter &&from)
        : _ptr(from._ptr)
        , _isConst(from._isConst)
        , _isOwn(from._isOwn)
    {
        if(from._ptr)
        {
            from._ptr = nullptr;
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    ValuePorter<T>::ValuePorter(const T &from)
        : _ptr(const_cast<T *>(&from))
        , _isConst(true)
        , _isOwn(false)
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    ValuePorter<T>::ValuePorter(T &&from)
        : _ptr(&from)
        , _isConst(false)
        , _isOwn(false)
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    ValuePorter<T>::~ValuePorter()
    {
        if(_ptr && _isOwn)
        {
            delete _ptr;
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    ValuePorter<T>::operator const T&() const
    {
        return *_ptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    ValuePorter<T>::operator T&&()
    {
        if(_isOwn)
        {
            assert(!_isConst);
            return std::move(*_ptr);
        }

        if(_isConst)
        {
            T *p = new T(*_ptr);
            _ptr = p;
            _isOwn = true;
            _isConst = false;
        }

        return std::move(*_ptr);
    }

}}}}

