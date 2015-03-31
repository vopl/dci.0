#pragma once

#include <dci/mm/sharedInstance.hpp>
#include "../event.hpp"

#include <type_traits>
#include <tuple>
#include <utility>

namespace dci { namespace async
{
    template <class E, class... T> class Future;
}}

namespace dci { namespace async { namespace details
{
    template <class E, class... T>
    struct FutureState
    {
        using Value = std::tuple<T...>;

        FutureState();
        ~FutureState();

        bool resolved() const;
        bool resolved2Value() const;
        bool resolved2Error() const;

        void resolveValue(T &&... val);
        void resolveError(E && error);

        void wait();
        Value &value();
        E &error();

    private:
        Event       _readyEvent;

        union
        {
            std::aligned_storage_t<sizeof(E), alignof(E)>   _error;
            std::aligned_storage_t<sizeof(Value), alignof(Value)>   _value;
            char _place[1];
        } _data;

        enum class DataState
        {
            null,
            error,
            value,
        } _dataState;
    };

    struct FutureStateAccessor
    {
        template <class... T>
        static FutureState<T...> &exec(Future<T...> &src)
        {
            return src.instance();
        }
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    FutureState<E, T...>::FutureState()
        : _readyEvent(false)
        , _dataState(DataState::null)
    {
    }

    template <class E, class... T>
    FutureState<E, T...>::~FutureState()
    {
        switch(_dataState)
        {
        case DataState::value:
            value().~Value();
            break;
        case DataState::error:
            error().~E();
            break;
        default:
            break;
        }
    }

    template <class E, class... T>
    bool FutureState<E, T...>::resolved() const
    {
        assert(_readyEvent.isSignalled() == (DataState::null != _dataState));
        return DataState::null != _dataState;
    }

    template <class E, class... T>
    bool FutureState<E, T...>::resolved2Value() const
    {
        return DataState::value == _dataState;
    }

    template <class E, class... T>
    bool FutureState<E, T...>::resolved2Error() const
    {
        return DataState::error == _dataState;
    }

    template <class E, class... T>
    void FutureState<E, T...>::resolveValue(T &&... val)
    {
        assert(!resolved());
        new(&_data._place) Value {std::forward<T>(val)...};
        _dataState = DataState::value;
        _readyEvent.set();
    }

    template <class E, class... T>
    void FutureState<E, T...>::resolveError(E && err)
    {
        assert(!resolved());
        new(&_data._place) E {std::forward<E>(err)};
        _dataState = DataState::error;
        _readyEvent.set();
    }

    template <class E, class... T>
    void FutureState<E, T...>::wait()
    {
        _readyEvent.acquire();
    }

    template <class E, class... T>
    typename FutureState<E, T...>::Value &FutureState<E, T...>::value()
    {
        return *reinterpret_cast<Value*>(&_data._place);
    }

    template <class E, class... T>
    E &FutureState<E, T...>::error()
    {
        return *reinterpret_cast<E*>(&_data._place);
    }
}}}
