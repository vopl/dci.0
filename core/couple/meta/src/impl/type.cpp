#include "type.hpp"

#include "alias.hpp"
#include "array.hpp"
#include "enum.hpp"
#include "errc.hpp"
#include "interface.hpp"
#include "list.hpp"
#include "map.hpp"
#include "primitive.hpp"
#include "ptr.hpp"
#include "set.hpp"
#include "struct.hpp"
#include "variant.hpp"


namespace dci { namespace couple { namespace meta { namespace impl
{
    Type::Type(TypeConcrete concrete)
        : _concrete (concrete)
    {
    }

    Type::~Type()
    {
    }

    TypeConcrete Type::concrete() const
    {
        return _concrete;
    }

    const runtime::Sign &Type::concreteSign() const
    {
        switch(concrete())
        {
        case TypeConcrete::null:
            ::abort();
        case TypeConcrete::alias:
            return static_cast<const Alias *>(this)->sign();
        case TypeConcrete::array:
            return static_cast<const Array *>(this)->sign();
        case TypeConcrete::enum_:
            return static_cast<const Enum *>(this)->sign();
        case TypeConcrete::errc:
            return static_cast<const Errc *>(this)->sign();
        case TypeConcrete::interface:
            return static_cast<const Interface *>(this)->sign();
        case TypeConcrete::list:
            return static_cast<const List *>(this)->sign();
        case TypeConcrete::map:
            return static_cast<const Map *>(this)->sign();
        case TypeConcrete::primitive:
            return static_cast<const Primitive *>(this)->sign();
        case TypeConcrete::ptr:
            return static_cast<const Ptr *>(this)->sign();
        case TypeConcrete::set:
            return static_cast<const Set *>(this)->sign();
        case TypeConcrete::struct_:
            return static_cast<const Struct *>(this)->sign();
        case TypeConcrete::variant:
            return static_cast<const Variant *>(this)->sign();
        }

        abort();
    }

}}}}
