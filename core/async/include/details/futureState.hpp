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
    struct FutureThenBase
    {
        FutureThenBase *_next;
        virtual void call(E *err, T *... vals) = 0;
        virtual void destroy() = 0;
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
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

        void pushThen(FutureThenBase<E, T...> *then);

    private:
        void execThens();
        void destroyThens();

    private:

        template <std::size_t... indices>
        static void expandCall(FutureThenBase<E, T...> *then, E &e, std::index_sequence<indices...>);

        template <std::size_t... indices>
        static void expandCall(FutureThenBase<E, T...> *then, Value &e, std::index_sequence<indices...>);


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

        FutureThenBase<E, T...> *_then;
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
        , _then(nullptr)
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
        destroyThens();
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
        execThens();
    }

    template <class E, class... T>
    void FutureState<E, T...>::resolveError(E && err)
    {
        assert(!resolved());
        new(&_data._place) E {std::forward<E>(err)};
        _dataState = DataState::error;
        _readyEvent.set();
        execThens();
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

    template <class E, class... T>
    void FutureState<E, T...>::pushThen(FutureThenBase<E, T...> *then)
    {
        then->_next = _then;
        _then = then;

        if(DataState::null != _dataState)
        {
            execThens();
        }
    }

    template <class E, class... T>
    void FutureState<E, T...>::execThens()
    {
        while(_then)
        {
            FutureThenBase<E, T...> *then = _then;
            _then = _then->_next;

            switch(_dataState)
            {
            case DataState::value:
                expandCall(then, value(), std::index_sequence_for<T...>());
                break;
            case DataState::error:
                expandCall(then, error(), std::index_sequence_for<T...>());
                break;
            default:
                break;
            }

            then->destroy();
        }
    }

    template <class E, class... T>
    void FutureState<E, T...>::destroyThens()
    {
        while(_then)
        {
            FutureThenBase<E, T...> *then = _then;
            _then = _then->_next;
            then->destroy();
        }
    }

    template <class E, class... T>
    template <std::size_t... indices>
    void FutureState<E, T...>::expandCall(FutureThenBase<E, T...> *then, E &e, std::index_sequence<indices...>)
    {
        then->call(&e, (std::tuple_element_t<indices, Value> *)nullptr...);
    }

    template <class E, class... T>
    template <std::size_t... indices>
    void FutureState<E, T...>::expandCall(FutureThenBase<E, T...> *then, Value &v, std::index_sequence<indices...>)
    {
        then->call(nullptr, &std::get<indices>(v)...);
    }

}}}
