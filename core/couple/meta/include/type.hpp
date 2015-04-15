#pragma once

#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")
#include <dci/himpl/faceLayout.hpp>

#include <type_traits>
#include <cstdlib>
#include <cassert>

namespace dci { namespace couple { namespace meta
{
    enum class TypeConcrete
    {
        null,
        alias,
        array,
        enum_,
        iface,
        list,
        map,
        primitive,
        ptr,
        set,
        struct_,
        variant
    };

    class Type
        : public himpl::FaceLayout<impl::Type>
    {
    public:
        TypeConcrete concrete() const;

        template <class T> T *cast();
        template <class T> const T *cast() const;

        template <class R, class F> R visit(F f);
        template <class R, class F> R visit(F f) const;
    };

    template <class R, class F> R Type::visit(F f)
    {
        switch(concrete())
        {
        case TypeConcrete::null:
            ::abort();
        case TypeConcrete::alias:
            return f(cast<Alias>());
        case TypeConcrete::array:
            return f(cast<Array>());
        case TypeConcrete::enum_:
            return f(cast<Enum>());
        case TypeConcrete::iface:
            return f(cast<Iface>());
        case TypeConcrete::list:
            return f(cast<List>());
        case TypeConcrete::map:
            return f(cast<Map>());
        case TypeConcrete::primitive:
            return f(cast<Primitive>());
        case TypeConcrete::ptr:
            return f(cast<Ptr>());
        case TypeConcrete::set:
            return f(cast<Set>());
        case TypeConcrete::struct_:
            return f(cast<Struct>());
        case TypeConcrete::variant:
            return f(cast<Variant>());
        }

        abort();
    }

    template <class R, class F> R Type::visit(F f) const
    {
        switch(concrete())
        {
        case TypeConcrete::null:
            abort();
        case TypeConcrete::alias:
            return f(cast<Alias>());
        case TypeConcrete::array:
            return f(cast<Array>());
        case TypeConcrete::enum_:
            return f(cast<Enum>());
        case TypeConcrete::iface:
            return f(cast<Iface>());
        case TypeConcrete::list:
            return f(cast<List>());
        case TypeConcrete::map:
            return f(cast<Map>());
        case TypeConcrete::primitive:
            return f(cast<Primitive>());
        case TypeConcrete::ptr:
            return f(cast<Ptr>());
        case TypeConcrete::set:
            return f(cast<Set>());
        case TypeConcrete::struct_:
            return f(cast<Struct>());
        case TypeConcrete::variant:
            return f(cast<Variant>());
        }

        abort();
    }

}}}
