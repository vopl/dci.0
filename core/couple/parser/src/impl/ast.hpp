#pragma once

#include "common.hpp"
#include "tokenizer.hpp"

#include <dci/couple/meta/libraryBuilder.hpp>

#include <boost/variant.hpp>


namespace dci { namespace couple { namespace parser { namespace impl
{
    /*
        primitive

        list
        set
        map
        ptr
        array

        name

        scopedName

        typeUse = primitive | list | set | map | ptr | array | scopedName

        alias

        bases

        variantField
        variant

        structField
        struct_

        enumField
        enum_

        errcField
        errc

        methodParam
        method
        interface

        scope

        include

        decl = alias | variant | struct | enum | errc | interface | scope

        decls = *decl | include



    */


    ////////////////////////////////////////////////////////////////////////////////
    // predecls

    //    primitive
    struct SPrimitive;
    using Primitive = std::shared_ptr<SPrimitive>;

    //    list
    struct SList;
    using List = std::shared_ptr<SList>;

    //    set
    struct SSet;
    using Set = std::shared_ptr<SSet>;

    //    map
    struct SMap;
    using Map = std::shared_ptr<SMap>;

    //    ptr
    struct SPtr;
    using Ptr = std::shared_ptr<SPtr>;

    //    array
    struct SArray;
    using Array = std::shared_ptr<SArray>;

    //    name
    struct SName;
    using Name = std::shared_ptr<SName>;

    //    scopedName
    struct SScopedName;
    using ScopedName = std::shared_ptr<SScopedName>;


    //    typeUse = primitive | list | set | map | ptr | array | scopedName
    using TypeUse = boost::variant<
          Primitive
        , List
        , Set
        , Map
        , Ptr
        , Array
        , ScopedName
    >;

    //    alias
    struct SAlias;
    using Alias = std::shared_ptr<SAlias>;

    //    bases
    struct SBaseStructs;
    using BaseStructs = std::shared_ptr<SBaseStructs>;

    struct SBaseInterfaces;
    using BaseInterfaces = std::shared_ptr<SBaseInterfaces>;

    //    structField
    struct SStructField;
    using StructField = std::shared_ptr<SStructField>;

    //    struct_
    struct SStruct;
    using Struct = std::shared_ptr<SStruct>;

    //    variantField
    struct SVariantField;
    using VariantField = std::shared_ptr<SVariantField>;

    //    variant
    struct SVariant;
    using Variant = std::shared_ptr<SVariant>;

    //    enumField
    struct SEnumField;
    using EnumField = std::shared_ptr<SEnumField>;

    //    enum_
    struct SEnum;
    using Enum = std::shared_ptr<SEnum>;

    //    errcField
    struct SErrcField;
    using ErrcField = std::shared_ptr<SErrcField>;

    //    errc
    struct SErrc;
    using Errc = std::shared_ptr<SErrc>;

    //    methodParam
    struct SMethodParam;
    using MethodParam = std::shared_ptr<SMethodParam>;

    //    method
    struct SMethod;
    using Method = std::shared_ptr<SMethod>;

    //    interface
    struct SInterface;
    using Interface = std::shared_ptr<SInterface>;

    struct SScopeEntry;

    //    scope
    struct SScope;
    using Scope = std::shared_ptr<SScope>;

    //    include

    //    decl = alias | variant | struct | enum | errc | interface | scope
    using Decl = boost::variant<
          Alias
        , Variant
        , Struct
        , Enum
        , Errc
        , Interface
        , Scope
    >;

    //    decls = *decl | include


    ////////////////////////////////////////////////////////////////////////////////
    //    primitive
    struct SPrimitive
    {
        meta::PrimitiveKind kind{meta::PrimitiveKind::void_};
        meta::Primitive     *meta{nullptr};

        SPrimitive() {}
        SPrimitive(meta::PrimitiveKind kind) : kind{kind} {}
    };

    //    list
    struct SList
    {
        TypeUse     elementType;

        meta::List  *meta{nullptr};
    };

    //    set
    struct SSet
    {
        TypeUse     elementType;

        meta::Set   *meta{nullptr};
    };

    //    map
    struct SMap
    {
        TypeUse     keyType;
        TypeUse     valueType;

        meta::Map   *meta{nullptr};
    };

    //    ptr
    struct SPtr
    {
        TypeUse     valueType;

        meta::Ptr   *meta{nullptr};
    };

    //    array
    struct SArray
    {
        TypeUse     elementType;
        std::string size;

        meta::Array *meta{nullptr};
    };

    //    name
    struct SName
    {
        Token::token_value_type pos;
        std::string             value;
    };

    //    scopedName
    struct SScopedName
    {
        Token::token_value_type pos;
        bool                    root{false};
        std::vector<Name>       values;

        boost::variant<
              SAlias *
            , SVariant *
            , SStruct *
            , SEnum *
            , SErrc *
            , SInterface *
        > asDecl;

        SScopeEntry *asScopedEntry;

        std::string toString()
        {
            std::string res = std::accumulate(
                        values.begin(),
                        values.end(),
                        std::string(),
                        [](const std::string &state, const Name &v){return state.empty() ? v->value : state+"::"+v->value;});

            return root ? "::"+res : res;
        }
    };

    //    typeUse = primitive | list | set | map | ptr | array | scopedName

    struct SScopeEntry
    {
        SScope  *owner{nullptr};
        Name    name;
    };

    //    scope
    struct SScope
        : SScopeEntry
    {
        std::vector<Decl>                   decls;

        std::map<std::string, SAlias *>     aliases;
        std::map<std::string, SStruct *>    structs;
        std::map<std::string, SVariant *>   variants;
        std::map<std::string, SEnum *>      enums;
        std::map<std::string, SErrc *>      errcs;
        std::map<std::string, SInterface *> interfaces;
        std::map<std::string, SScope *>     scopes;

        meta::Scope                         *meta{nullptr};
    };

    //    alias
    struct SAlias
        : SScopeEntry
    {
        TypeUse     type;

        meta::Alias *meta{nullptr};
    };

    //    bases
    struct SBaseStructs
    {
        std::vector<ScopedName> scopedNames;
        std::vector<SStruct*>   instances;
    };

    struct SBaseInterfaces
    {
        std::vector<ScopedName>     scopedNames;
        std::vector<SInterface*>    instances;
    };

    //    variantField
    struct SVariantField
    {
        SVariant        *owner{nullptr};
        Name            name;
        TypeUse         type;

        meta::Attribute *meta{nullptr};
    };

    //    variant
    struct SVariant
        : SScope
    {
        std::vector<VariantField>   fields;

        meta::Variant               *meta{nullptr};
    };

    //    structField
    struct SStructField
    {
        SStruct         *owner{nullptr};
        Name            name;
        TypeUse         type;

        meta::Attribute *meta{nullptr};
    };

    //    struct_
    struct SStruct
        : SScope
    {
        BaseStructs                 bases;
        std::vector<StructField>    fields;

        meta::Struct                *meta{nullptr};
    };

    //    enumField
    struct SEnumField
    {
        SEnum           *owner{nullptr};
        Name            name;
        std::string     value;

        meta::EnumValue *meta{nullptr};
    };

    //    enum_
    struct SEnum
        : SScopeEntry
    {
        std::vector<EnumField>  fields;

        meta::Enum              *meta{nullptr};
    };

    //    errcField
    struct SErrcField
    {
        SErrc           *owner{nullptr};
        Name            name;

        std::string     description;

        meta::ErrcValue *meta{nullptr};
    };

    //    errc
    struct SErrc
        : SScopeEntry
    {
        std::vector<ErrcField>  fields;

        meta::Errc              *meta{nullptr};
    };

    //    methodParam
    struct SMethodParam
    {
        SMethod         *owner{nullptr};
        Name            name;
        TypeUse         type;

        meta::Attribute *meta{nullptr};
    };

    //    method
    enum class MethodDirection
    {
        in,
        out
    };

    struct SMethod
    {
        SInterface                  *owner{nullptr};

        MethodDirection             direction{MethodDirection::out};
        Name                        name;
        std::vector<MethodParam>    query;
        bool                        noreply{false};
        std::vector<TypeUse>        reply;

        meta::Method                *meta{nullptr};
    };

    //    interface
    struct SInterface
        : SScope
    {
        bool                        primary{true};
        std::weak_ptr<SInterface>   opposite;
        BaseInterfaces              bases;
        std::vector<Method>         methods;

        meta::Interface             *meta{nullptr};
    };

    //    include

    //    decl = alias | variant | struct | enum | errc | interface

    //    decls = *(decl | include)

}}}}
