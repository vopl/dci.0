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

    struct NullRemoveCleaner
    {
        static constexpr bool _isNull {true};
    };

    template <class Cleaner>
    class RemoveCleanExecutor
        : private Cleaner
    {
    public:
        static constexpr bool _isNull {false};

        template <class... T>
        void operator()(T &&... args)
        {
            Cleaner::operator()(std::forward<T>(args)...);
        }
    };

    template <>
    class RemoveCleanExecutor<NullRemoveCleaner>
    {
    public:
        template <class... T>
        void operator()(const T&...) {}
    };

    template <class T, class RemoveCleaner=NullRemoveCleaner>
    class IntrusiveDlist
        : private RemoveCleanExecutor<RemoveCleaner>
    {
        IntrusiveDlist(const IntrusiveDlist &) = delete;
        void operator=(const IntrusiveDlist &) = delete;

    public:
        IntrusiveDlist();
        IntrusiveDlist(T *element);
        IntrusiveDlist(RemoveCleaner &&removeCleaner);
        IntrusiveDlist(T *element, RemoveCleaner &&removeCleaner);
        ~IntrusiveDlist();

    public:
        bool empty() const;
        T *first() const;
        T *last() const;
        std::pair<T*, T*> range() const;
        bool contain(T *element) const;
        void push(T *element);
        void remove(T *element);

        void clear();

        template <class F>
        void each(F &&f);

        template <class F>
        void each(F &&f) const;

        template <class F>
        void flush(F &&f);

    private:
        IntrusiveDlistElement<T> _first;
        IntrusiveDlistElement<T> _last;
    };
}}

#include "intrusiveDlist.ipp"
