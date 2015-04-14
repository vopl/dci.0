#include <type.hpp>
#include "impl/type.hpp"

#include <alias.hpp>
#include "impl/alias.hpp"

#include <array.hpp>
#include "impl/array.hpp"

#include <enum.hpp>
#include "impl/enum.hpp"

#include <iface.hpp>
#include "impl/iface.hpp"

#include <list.hpp>
#include "impl/list.hpp"

#include <map.hpp>
#include "impl/map.hpp"

#include <primitive.hpp>
#include "impl/primitive.hpp"

#include <ptr.hpp>
#include "impl/ptr.hpp"

#include <set.hpp>
#include "impl/set.hpp"

#include <struct.hpp>
#include "impl/struct.hpp"

#include <variant.hpp>
#include "impl/variant.hpp"

#include <cassert>

namespace dci { namespace couple { namespace meta
{
    TypeConcrete Type::concrete() const
    {
        return impl().concrete();
    }

    template <class T> T *Type::cast()
    {
        assert(T::_concrete == concrete());
        return himpl::impl2Face<T>(static_cast<typename T::Impl*>(pimpl()));
    }

    template <class T> const T *Type::cast() const
    {
        assert(T::_concrete == concrete());
        return himpl::impl2Face<T>(static_cast<const typename T::Impl*>(pimpl()));
    }


    template Alias *Type::cast<Alias>();
    template Array *Type::cast<Array>();
    template Enum *Type::cast<Enum>();
    template Iface *Type::cast<Iface>();
    template List *Type::cast<List>();
    template Map *Type::cast<Map>();
    template Primitive *Type::cast<Primitive>();
    template Ptr *Type::cast<Ptr>();
    template Set *Type::cast<Set>();
    template Struct *Type::cast<Struct>();
    template Variant *Type::cast<Variant>();


    template const Alias *Type::cast<Alias>() const;
    template const Array *Type::cast<Array>() const;
    template const Enum *Type::cast<Enum>() const;
    template const Iface *Type::cast<Iface>() const;
    template const List *Type::cast<List>() const;
    template const Map *Type::cast<Map>() const;
    template const Primitive *Type::cast<Primitive>() const;
    template const Ptr *Type::cast<Ptr>() const;
    template const Set *Type::cast<Set>() const;
    template const Struct *Type::cast<Struct>() const;
    template const Variant *Type::cast<Variant>() const;

}}}
