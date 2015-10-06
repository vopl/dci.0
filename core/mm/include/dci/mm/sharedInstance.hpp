#pragma once

#include "functions.hpp"

#include <type_traits>
#include <cstdint>
#include <cassert>
#include <utility>

namespace dci { namespace mm
{

    struct SharedInstanceNullInitializer{};

    template <class T>
    class SharedInstance
    {
    public:
        using Instance = typename std::decay<T>::type;

    public:
        SharedInstance(SharedInstanceNullInitializer);

        template <class... Args>
        SharedInstance(Args &&... args);

        SharedInstance(const SharedInstance &other);
        SharedInstance(SharedInstance &&other);

        ~SharedInstance();

        SharedInstance &operator=(const SharedInstance &other);
        SharedInstance &operator=(SharedInstance &&other);

        operator bool() const;

        std::int32_t counter() const;

        Instance &instance();
        Instance *instancePtr();
        Instance *operator->();

        const Instance &instance() const;
        const Instance *instancePtr() const;
        const Instance *operator->() const;

    private:
        void ref();
        void unref();

    protected:
        struct State
        {
            Instance        _instance;
            std::uint32_t   _counter;
        };

        State *_state;
    };


    template <class T>
    SharedInstance<T>::SharedInstance(SharedInstanceNullInitializer)
        : _state(nullptr)
    {
    }

    template <class T>
    template <class... Args>
    SharedInstance<T>::SharedInstance(Args &&... args)
        : _state(static_cast<State *>(mm::alloc<sizeof(State)>()))
    {
        new(&_state->_instance) Instance(std::forward<Args...>(args)...);
        _state->_counter = 1;
    }

    template <class T>
    SharedInstance<T>::SharedInstance(const SharedInstance &other)
        : _state(other._state)
    {
        ref();
    }

    template <class T>
    SharedInstance<T>::SharedInstance(SharedInstance &&other)
        : _state(other._state)
    {
        if(other._state)
        {
            other._state = nullptr;
        }
    }

    template <class T>
    SharedInstance<T>::~SharedInstance()
    {
        unref();
    }

    template <class T>
    SharedInstance<T> &SharedInstance<T>::operator=(const SharedInstance &other)
    {
        unref();
        _state = other._state;
        ref();

        return *this;
    }

    template <class T>
    SharedInstance<T> &SharedInstance<T>::operator=(SharedInstance &&other)
    {
        unref();
        _state = other._state;
        if(other._state)
        {
            other._state = nullptr;
        }

        return *this;
    }

    template <class T>
    SharedInstance<T>::operator bool() const
    {
        return !!_state;
    }

    template <class T>
    std::int32_t SharedInstance<T>::counter() const
    {
        if(_state)
        {
            assert(_state->_counter);
            return _state->_counter;
        }

        return 0;
    }

    template <class T>
    typename SharedInstance<T>::Instance &SharedInstance<T>::instance()
    {
        assert(_state);
        return _state->_instance;
    }

    template <class T>
    typename SharedInstance<T>::Instance *SharedInstance<T>::instancePtr()
    {
        assert(_state);
        return &_state->_instance;
    }

    template <class T>
    typename SharedInstance<T>::Instance *SharedInstance<T>::operator->()
    {
        return instancePtr();
    }

    template <class T>
    const typename SharedInstance<T>::Instance &SharedInstance<T>::instance() const
    {
        assert(_state);
        return _state->_instance;
    }

    template <class T>
    const typename SharedInstance<T>::Instance *SharedInstance<T>::instancePtr() const
    {
        assert(_state);
        return &_state->_instance;
    }

    template <class T>
    const typename SharedInstance<T>::Instance *SharedInstance<T>::operator->() const
    {
        return instancePtr();
    }

    template <class T>
    void SharedInstance<T>::ref()
    {
        if(_state)
        {
            assert(_state->_counter);
            _state->_counter++;
        }
    }

    template <class T>
    void SharedInstance<T>::unref()
    {
        if(_state)
        {
            if(! --_state->_counter)
            {
                _state->_instance.~Instance();
                mm::free<sizeof(State)>(_state);
                _state = nullptr;
            }
        }
    }

}}
