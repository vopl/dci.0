#pragma once

#include "alias.hpp"
#include "array.hpp"
#include "enum.hpp"
#include "errc.hpp"
#include "interface.hpp"
#include "list.hpp"
#include "map.hpp"
#include "ptr.hpp"
#include "primitive.hpp"
#include "scope.hpp"
#include "set.hpp"
#include "struct.hpp"
#include "variant.hpp"


#include <set>
#include <memory>
#include <cassert>

namespace dci { namespace couple { namespace meta { namespace impl
{
    class LibraryContent
    {
        LibraryContent(const LibraryContent &) = delete;
        void operator=(const LibraryContent &) = delete;

    public:
        LibraryContent();
        LibraryContent(LibraryContent &&from);
        ~LibraryContent();

        LibraryContent &operator=(LibraryContent &&from);

    public:
        template <class T>
        using Items = std::set<T *>;

        template <class T>
        T *create();

        template <class T>
        void locate(T *p);

        template <class T>
        Items<T> &items();

        template <class T>
        const Items<T> &items() const;

        template <class T>
        void checkPresense(T *p);

        void clear();

    private:
        using Holder = std::unique_ptr<void, void(*)(void*)>;
        std::vector<Holder> _holders;

        Items<Type>         _types;
        Items<List>         _lists;
        Items<Set>          _sets;
        Items<Map>          _maps;
        Items<Ptr>          _ptrs;
        Items<Array>        _arrays;
        Items<Primitive>    _primitives;
        Items<Scope>        _scopes;
        Items<Alias>        _aliases;
        Items<Struct>       _structs;
        Items<Variant>      _variants;
        Items<Enum>         _enums;
        Items<Errc>         _errcs;
        Items<Interface>        _interfaces;
        Items<Method>       _methods;
        Items<Attribute>    _attributes;
        Items<EnumValue>    _enumValues;
        Items<ErrcValue>    _errcValues;
    };

    template <class T>
    T *LibraryContent::create()
    {
        T *res = new T;
        _holders.emplace(_holders.end(), res, [](void *p){delete static_cast<T *>(p);});

        locate(res);

        return res;
    }

    namespace
    {
        template <class T, class TItems>
        typename std::enable_if<std::is_convertible<T*, typename TItems::value_type>::value>::type locateConvertible(T *p, TItems &items)
        {
            items.emplace(p);
        }

        template <class T, class TItems>
        typename std::enable_if<!std::is_convertible<T*, typename TItems::value_type>::value>::type locateConvertible(T *, TItems &)
        {
        }
    }

    template <class T>
    const LibraryContent::Items<T> &LibraryContent::items() const
    {
        return const_cast<LibraryContent *>(this)->items<T>();
    }

    template <class T>
    void LibraryContent::locate(T *p)
    {
        locateConvertible(p, items<Type>());
        locateConvertible(p, items<List>());
        locateConvertible(p, items<Set>());
        locateConvertible(p, items<Map>());
        locateConvertible(p, items<Ptr>());
        locateConvertible(p, items<Array>());
        locateConvertible(p, items<Primitive>());
        locateConvertible(p, items<Scope>());
        locateConvertible(p, items<Alias>());
        locateConvertible(p, items<Struct>());
        locateConvertible(p, items<Variant>());
        locateConvertible(p, items<Enum>());
        locateConvertible(p, items<Errc>());
        locateConvertible(p, items<Interface>());
        locateConvertible(p, items<Method>());
        locateConvertible(p, items<Attribute>());
        locateConvertible(p, items<EnumValue>());
        locateConvertible(p, items<ErrcValue>());
    }

    template <class T>
    void LibraryContent::checkPresense(T *p)
    {
        if(items<T>().end() == items<T>().find(p))
        {
            assert(0&&"requested object is not presented in builder");
            abort();
        }
    }

}}}}
