#pragma once

#include <dci/couple/meta/libraryBuilder.hpp>
#include "libraryContent.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class LibraryBuilder
    {
    public:
        LibraryBuilder();
        ~LibraryBuilder();

    public:
        void merge(const Library &lib);

    public:// changes
        Primitive   *addPrimitive();
        List        *addList();
        Set         *addSet();
        Map         *addMap();
        Ptr         *addPtr();
        Array       *addArray();

        Scope       *addScope(Scope *parent, const std::string &name);
        Alias       *addAlias(Scope *parent, const std::string &name);
        Struct      *addStruct(Scope *parent, const std::string &name);
        Variant     *addVariant(Scope *parent, const std::string &name);
        Enum        *addEnum(Scope *parent, const std::string &name);
        Errc        *addErrc(Scope *parent, const std::string &name);
        Interface       *addInterface(Scope *parent, const std::string &name);

        Method      *addMethod(Interface *parent, const std::string &name);

        Attribute   *addAttribute(Struct *parent, const std::string &name);
        Attribute   *addAttribute(Variant *parent, const std::string &name);
        Attribute   *addAttribute(Method *parent, const std::string &name);
        EnumValue   *addEnumValue(Enum *parent, const std::string &name);
        ErrcValue   *addErrcValue(Errc *parent, const std::string &name);

        void addBase(Struct *target, Struct *base);
        void addBase(Interface *target, Interface *base);

        void setType(Attribute *target, Type *type);
        void setType(Alias *target, Type *type);

        void setElementType(Set *target, Type *type);
        void setElementType(List *target, Type *type);
        void setElementType(Array *target, Type *type);
        void setElementType(Ptr *target, Type *type);

        void setElementType(Map *target, Type *keyType, Type *valueType);

        void setMethodDirection(Method *target, CallDirection direction);
        void setMethodNoreply(Method *target, bool noreply);
        void addReplyArg(Method *target, Type *type);

        void setArraySize(Array *target, std::uint32_t size);

        void setPrimitiveKind(Primitive *target, PrimitiveKind kind);

        void setDescription(ErrcValue *target, const std::string &description);
        void setValue(EnumValue *target, const std::string &description);

        void setInterfacePrimary(Interface *target, bool primary);
        void setInterfaceOpposite(Interface *target, Interface *opposite);

    public:// commit
        bool commitChanges(Library &lib, std::vector<meta::CommitError> &errors);
        void rollbackChanges();

    private:
        LibraryContent _lc;
    };

}}}}
