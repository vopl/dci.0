#pragma once

#include <type_traits>
#include <cassert>

namespace dci { namespace couple { namespace runtime { namespace call
{

    struct move_not_copy { move_not_copy(move_not_copy &&); };

    template <class T, bool isClass = std::is_class<T>::value> struct helper;

    template <class T>
    struct helper<T, true>
        : public move_not_copy
        , public T
    {
    };

    template <class T>
    struct helper<T, false>
    {
        helper(helper &&) = delete;
    };

    template<typename T>
    struct has_move_constructor
    {
        static const bool value = std::is_move_constructible<helper<T>>::value;

        constexpr operator bool () const { return value; }
    };

    template <class T>
    struct Value
    {
        //is big
        //has default ctor
        //has move ctor
        //has move =
        //has copy ctor
        //has copy =
        //has destructor

        Value()
        {
            assert(0);
        }

        Value(const Value &v)
        {
            assert(0);
        }

        Value(Value &&)
        {
            assert(0);
        }

        Value(const T &)
        {
            assert(0);
        }

        Value(T &&v)
        {
            assert(0);
        }

        ~Value()
        {
            assert(0);
        }


        Value &operator=(const Value &);
        Value &operator=(Value &&);


        Value &operator=(const T &);
        Value &operator=(T &&);

        operator const T&() const
        {
            assert(0);
        }

        operator T&()
        {
            assert(0);
        }

        operator T&&()
        {
            assert(0);
        }

    };

}}}}

