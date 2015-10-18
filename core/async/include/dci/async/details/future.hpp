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

namespace dci { namespace async { namespace details { namespace future
{

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    struct ThenBase
    {
        ThenBase *_next;
        virtual ~ThenBase() {}
        virtual void call(Future<E, T...> &) = 0;
        virtual void destroy() = 0;
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    struct State
    {
        State();
        ~State();

        using Value = std::tuple<T...>;

        union Data
        {
            E       _error;
            Value   _value;

            Data() {}
            ~Data() {}
        };

        enum class Mode
        {
            null,
            error,
            value,
        };

        Event               _readyEvent     {};
        ThenBase<E, T...> * _then           {nullptr};
        Mode                _mode           {Mode::null};
        Data                _data           {};
    };


    struct EngineNullInitializer {};

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    struct Engine
        : public ::dci::mm::SharedInstance<State<E, T...>>
    {
        using State = future::State<E, T...>;
        using Value = typename State::Value;
        using Mode = typename State::Mode;
        using Data = typename State::Data;

        Engine(EngineNullInitializer);
        Engine();
        Engine(const Engine &);
        Engine(Engine &&);
        ~Engine();


        Engine &operator=(const Engine &);
        Engine &operator=(Engine &&);


        bool resolved() const;
        bool resolved2Value() const;
        bool resolved2Error() const;

        void resolveValue(T &&... val);
        void resolveValue(Value &&val);
        void resolveValue(const Value &val);
        void resolveError(E && error);

        void pushThen(ThenBase<E, T...> *then);

        Value &value();
        E &error();
        Waitable *waitable();

    private:
        State &state();
        const State &state() const;

        void execThens();
    };



    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    State<E, T...>::State()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    State<E, T...>::~State()
    {
        switch(_mode)
        {
        case Mode::value:
            _data._value.~Value();
            break;
        case Mode::error:
            _data._error.~E();
            break;
        case Mode::null:
        default:
            break;
        }

        while(_then)
        {
            ThenBase<E, T...> *then = _then;
            _then = _then->_next;
            then->destroy();
        }
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    Engine<E, T...>::Engine(EngineNullInitializer)
        : ::dci::mm::SharedInstance<State>(mm::SharedInstanceNullInitializer())
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    Engine<E, T...>::Engine()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    Engine<E, T...>::~Engine()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    Engine<E, T...>::Engine(const Engine &from)
        : ::dci::mm::SharedInstance<State>(static_cast<const ::dci::mm::SharedInstance<State> &>(from))
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    Engine<E, T...>::Engine(Engine &&from)
        : ::dci::mm::SharedInstance<State>(std::forward< ::dci::mm::SharedInstance<State>>(from))
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    Engine<E, T...> &Engine<E, T...>::operator=(const Engine &from)
    {
        ::dci::mm::SharedInstance<State>::operator =(static_cast<const ::dci::mm::SharedInstance<State> &>(from));
        return *this;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    Engine<E, T...> &Engine<E, T...>::operator=(Engine &&from)
    {
        ::dci::mm::SharedInstance<State>::operator =(std::forward< ::dci::mm::SharedInstance<State>>(from));
        return *this;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    bool Engine<E, T...>::resolved() const
    {
        assert(state()._readyEvent.canAcquire() == (Mode::null != state()._mode));
        return Mode::null != state()._mode;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    bool Engine<E, T...>::resolved2Value() const
    {
        assert(state()._readyEvent.canAcquire() == (Mode::null != state()._mode));
        return Mode::value == state()._mode;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    bool Engine<E, T...>::resolved2Error() const
    {
        assert(state()._readyEvent.canAcquire() == (Mode::null != state()._mode));
        return Mode::error == state()._mode;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    void Engine<E, T...>::resolveValue(T &&... val)
    {
        assert(!resolved());
        new(&value()) Value {std::forward<T>(val)...};
        state()._mode = Mode::value;
        state()._readyEvent.set();
        execThens();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    void Engine<E, T...>::resolveValue(Value &&val)
    {
        assert(!resolved());
        new(&value()) Value {std::move(val)};
        state()._mode = Mode::value;
        state()._readyEvent.set();
        execThens();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    void Engine<E, T...>::resolveValue(const Value &val)
    {
        assert(!resolved());
        new(&value()) Value {val};
        state()._mode = Mode::value;
        state()._readyEvent.set();
        execThens();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    void Engine<E, T...>::resolveError(E &&err)
    {
        assert(!resolved());
        new(&error()) E {std::forward<E>(err)};
        state()._mode = Mode::error;
        state()._readyEvent.set();
        execThens();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    void Engine<E, T...>::pushThen(ThenBase<E, T...> *then)
    {
        then->_next = state()._then;
        state()._then = then;

        if(Mode::null != state()._mode)
        {
            execThens();
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    typename Engine<E, T...>::Value &Engine<E, T...>::value()
    {
        return state()._data._value;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    E &Engine<E, T...>::error()
    {
        return state()._data._error;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    Waitable *Engine<E, T...>::waitable()
    {
        return &state()._readyEvent;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    typename Engine<E, T...>::State &Engine<E, T...>::state()
    {
        return this->instance();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    const typename Engine<E, T...>::State &Engine<E, T...>::state() const
    {
        return this->instance();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... T>
    void Engine<E, T...>::execThens()
    {
        if(state()._then)
        {
            Future<E, T...> ftr(*this);

            while(state()._then)
            {
                ThenBase<E, T...> *then = state()._then;
                state()._then = state()._then->_next;

                then->call(ftr);
                then->destroy();
            }
        }
    }

}}}}
