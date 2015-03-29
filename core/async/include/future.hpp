#pragma once

#include <dci/mm/sharedInstance.hpp>
#include "event.hpp"

#include <type_traits>
#include <tuple>
#include <utility>
#include <exception>

namespace dci { namespace async
{
    template <typename... T>
    class Future;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    namespace details
    {
        template <typename... T>
        struct FutureState
        {
            using Value = std::tuple<T...>;

            FutureState();
            ~FutureState();

            bool resolved() const;
            void resolve(T &&... val);

            void wait();
            Value &value();

        private:
            Event       _readyEvent;
            typename std::aligned_storage<sizeof(Value), alignof(Value)>::type   _valueArea;
        };

        struct FutureStateAccessor
        {
            template <typename... T>
            static FutureState<T...> &exec(Future<T...> &src)
            {
                return src.instance();
            }
        };
    }

    template <typename... T> class Promise;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename... T>
    class Future
        : private dci::mm::SharedInstance<details::FutureState<T...>>
    {
        friend class Promise<T...>;
        friend class details::FutureStateAccessor;
        using StateInstance = dci::mm::SharedInstance<details::FutureState<T...>>;
        Future(const StateInstance &state);

    public:
        Future(T &&... vals);
        Future(const Future &other);
        Future(Future &&other);

        Future &operator=(const Future &other);
        Future &operator=(Future &&other);

        void wait();
        bool resolved() const;

        const std::tuple<T...> &value();

        template <std::size_t idx>
        const typename std::tuple_element<idx, std::tuple<T...> >::type &value();

        std::tuple<T...> &&detachValue();

        template <std::size_t idx>
        typename std::tuple_element<idx, std::tuple<T...> >::type &&detachValue();
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename... T>
    class Promise
        : private dci::mm::SharedInstance<details::FutureState<T...>>
    {
        using StateInstance = dci::mm::SharedInstance<details::FutureState<T...>>;

        Promise(const Promise &other) = delete;
        Promise &operator=(const Promise &other) = delete;

    public:
        Promise();
        Promise(Promise &&other);
        Promise &operator=(Promise &&other);

        ~Promise();

        Future<T...> future();

        bool resolved() const;
        void resolve(T&&... val);
    };

    template <typename... T>
    Future<T...> mkReadyFuture(T&&... val)
    {
        return Future<T...> {std::forward<T>(val)...};
    }

    namespace details
    {
        template <typename... T>
        FutureState<T...>::FutureState()
            : _readyEvent(false)
        {
        }

        template <typename... T>
        FutureState<T...>::~FutureState()
        {
            if(resolved())
            {
                value().~Value();
            }
        }

        template <typename... T>
        bool FutureState<T...>::resolved() const
        {
            return _readyEvent.isSignalled();
        }

        template <typename... T>
        void FutureState<T...>::resolve(T &&... val)
        {
            assert(!resolved());
            new(&value()) Value {std::forward<T>(val)...};
            _readyEvent.set();
        }

        template <typename... T>
        void FutureState<T...>::wait()
        {
            _readyEvent.acquire();
        }

        template <typename... T>
        typename FutureState<T...>::Value &FutureState<T...>::value()
        {
            return *reinterpret_cast<Value *>(&_valueArea);
        }
    }


    template <typename... T>
    Future<T...>::Future(T &&... vals)
        : StateInstance()
    {
        this->instance().resolve(std::forward<T>(vals)...);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename... T>
    Future<T...>::Future(const StateInstance &state)
        : StateInstance(state)
    {
    }

    template <typename... T>
    Future<T...>::Future(const Future &other)
        : StateInstance(static_cast<const StateInstance &>(other))
    {
    }

    template <typename... T>
    Future<T...>::Future(Future &&other)
        : StateInstance(std::forward<StateInstance>(other))
    {

    }

    template <typename... T>
    Future<T...> &Future<T...>::operator=(const Future &other)
    {
        this->StateInstance::operator=(other);
        return *this;
    }

    template <typename... T>
    Future<T...> &Future<T...>::operator=(Future &&other)
    {
        this->StateInstance::operator=(std::forward<StateInstance>(other));
        return *this;
    }

    template <typename... T>
    void Future<T...>::wait()
    {
        this->instance().wait();
    }

    template <typename... T>
    bool Future<T...>::resolved() const
    {
        return this->instance().resolved();
    }

    template <typename... T>
    const std::tuple<T...> &Future<T...>::value()
    {
        wait();
        return this->instance().value();
    }

    template <typename... T>
    template <std::size_t idx>
    const typename std::tuple_element<idx, std::tuple<T...> >::type &Future<T...>::value()
    {
        wait();
        return std::get<idx>(this->instance().value());
    }

    template <typename... T>
    std::tuple<T...> &&Future<T...>::detachValue()
    {
        wait();
        return this->value();
    }


    template <typename... T>
    template <std::size_t idx>
    typename std::tuple_element<idx, std::tuple<T...> >::type &&Future<T...>::detachValue()
    {
        wait();
        return std::get<idx>(std::forward<typename details::FutureState<T...>::Value>(this->instance().value()));
    }



    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename... T>
    Promise<T...>::Promise()
        : StateInstance()
    {
    }

    template <typename... T>
    Promise<T...>::Promise(Promise &&other)
        : StateInstance(std::forward<StateInstance>(other))
    {
    }

    template <typename... T>
    Promise<T...> &Promise<T...>::operator=(Promise &&other)
    {
        this->StateInstance::operator=( std::forward<StateInstance>(other));
    }

    template <typename... T>
    Promise<T...>::~Promise()
    {
        if(this->counter()>1 && !this->instance().resolved())
        {
            assert(!"unsetted promise destroyed while futures exists");
            std::abort();
        }
    }

    template <typename... T>
    Future<T...> Promise<T...>::future()
    {
        return Future<T...>(*this);
    }

    template <typename... T>
    bool Promise<T...>::resolved() const
    {
        return this->instance().resolved();
    }

    template <typename... T>
    void Promise<T...>::resolve(T&&... val)
    {
        this->instance().resolve(std::forward<T>(val)...);
    }

}}
