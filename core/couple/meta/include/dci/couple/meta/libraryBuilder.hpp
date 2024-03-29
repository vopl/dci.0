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

#include <string>
#include <system_error>

namespace dci { namespace couple { namespace meta
{
    struct APIDCI_COUPLE_META CommitError
    {
        std::error_code _ec;
        std::string     _comment;

        std::string toString() const;
    };


    class APIDCI_COUPLE_META LibraryBuilder
        : public himpl::FaceLayout<impl::LibraryBuilder>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        LibraryBuilder();
        ~LibraryBuilder();

    public:// changes
        //LoadResult load(const std::string &file);
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
        void setValue(EnumValue *target, const std::string &value);

        void setInterfacePrimary(Interface *target, bool primary);
        void setInterfaceOpposite(Interface *target, Interface *opposite);

    public:// commit
        bool commitChanges(Library &lib, std::vector<CommitError> &errors);
        void rollbackChanges();

    };
}}}
