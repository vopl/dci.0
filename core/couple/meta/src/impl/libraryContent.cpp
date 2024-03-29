#include "libraryContent.hpp"
#include "scope.hpp"

#include <type_traits>

namespace dci { namespace couple { namespace meta { namespace impl
{
    LibraryContent::LibraryContent()
    {
    }

    LibraryContent::LibraryContent(LibraryContent &&from)
        : _holders(std::move(from._holders))
        , _types(std::move(from._types))
        , _lists(std::move(from._lists))
        , _sets(std::move(from._sets))
        , _maps(std::move(from._maps))
        , _ptrs(std::move(from._ptrs))
        , _arrays(std::move(from._arrays))
        , _primitives(std::move(from._primitives))
        , _scopes(std::move(from._scopes))
        , _aliases(std::move(from._aliases))
        , _structs(std::move(from._structs))
        , _variants(std::move(from._variants))
        , _enums(std::move(from._enums))
        , _errcs(std::move(from._errcs))
        , _interfaces(std::move(from._interfaces))
        , _methods(std::move(from._methods))
        , _attributes(std::move(from._attributes))
        , _enumValues(std::move(from._enumValues))
        , _errcValues(std::move(from._errcValues))
    {
    }

    LibraryContent::~LibraryContent()
    {
    }

    LibraryContent &LibraryContent::operator=(LibraryContent &&from)
    {
        _holders = (std::move(from._holders));
        _types = (std::move(from._types));
        _lists = (std::move(from._lists));
        _sets = (std::move(from._sets));
        _maps = (std::move(from._maps));
        _ptrs = (std::move(from._ptrs));
        _arrays = (std::move(from._arrays));
        _primitives = (std::move(from._primitives));
        _scopes = (std::move(from._scopes));
        _aliases = (std::move(from._aliases));
        _structs = (std::move(from._structs));
        _variants = (std::move(from._variants));
        _enums = (std::move(from._enums));
        _errcs = (std::move(from._errcs));
        _interfaces = (std::move(from._interfaces));
        _methods = (std::move(from._methods));
        _attributes = (std::move(from._attributes));
        _enumValues = (std::move(from._enumValues));
        _errcValues = (std::move(from._errcValues));

        return *this;
    }

    template <>
    LibraryContent::Items<Type> &LibraryContent::items<Type>()
    {
        return _types;
    }

    template <>
    LibraryContent::Items<List> &LibraryContent::items<List>()
    {
        return _lists;
    }

    template <>
    LibraryContent::Items<Set> &LibraryContent::items<Set>()
    {
        return _sets;
    }

    template <>
    LibraryContent::Items<Map> &LibraryContent::items<Map>()
    {
        return _maps;
    }

    template <>
    LibraryContent::Items<Ptr> &LibraryContent::items<Ptr>()
    {
        return _ptrs;
    }

    template <>
    LibraryContent::Items<Array> &LibraryContent::items<Array>()
    {
        return _arrays;
    }

    template <>
    LibraryContent::Items<Primitive> &LibraryContent::items<Primitive>()
    {
        return _primitives;
    }

    template <>
    LibraryContent::Items<Scope> &LibraryContent::items<Scope>()
    {
        return _scopes;
    }

    template <>
    LibraryContent::Items<Alias> &LibraryContent::items<Alias>()
    {
        return _aliases;
    }

    template <>
    LibraryContent::Items<Struct> &LibraryContent::items<Struct>()
    {
        return _structs;
    }

    template <>
    LibraryContent::Items<Variant> &LibraryContent::items<Variant>()
    {
        return _variants;
    }

    template <>
    LibraryContent::Items<Enum> &LibraryContent::items<Enum>()
    {
        return _enums;
    }

    template <>
    LibraryContent::Items<Errc> &LibraryContent::items<Errc>()
    {
        return _errcs;
    }

    template <>
    LibraryContent::Items<Interface> &LibraryContent::items<Interface>()
    {
        return _interfaces;
    }

    template <>
    LibraryContent::Items<Method> &LibraryContent::items<Method>()
    {
        return _methods;
    }

    template <>
    LibraryContent::Items<Attribute> &LibraryContent::items<Attribute>()
    {
        return _attributes;
    }

    template <>
    LibraryContent::Items<EnumValue> &LibraryContent::items<EnumValue>()
    {
        return _enumValues;
    }

    template <>
    LibraryContent::Items<ErrcValue> &LibraryContent::items<ErrcValue>()
    {
        return _errcValues;
    }

    void LibraryContent::clear()
    {
        _lists.clear();
        _sets.clear();
        _maps.clear();
        _ptrs.clear();
        _arrays.clear();
        _primitives.clear();
        _scopes.clear();
        _aliases.clear();
        _structs.clear();
        _variants.clear();
        _enums.clear();
        _errcs.clear();
        _interfaces.clear();
        _methods.clear();
        _attributes.clear();
        _enums.clear();
        _errcs.clear();

        _holders.clear();
    }

}}}}
