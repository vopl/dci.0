#include "executorCpp.hpp"

#include <boost/filesystem.hpp>
#include <iostream>
#include <cassert>

namespace dci { namespace couple { namespace generator { namespace impl
{
    using namespace dci::couple::meta;

    ExecutorCpp::ExecutorCpp()
    {
    }

    ExecutorCpp::~ExecutorCpp()
    {
    }

    bool ExecutorCpp::exec(
        const Library &lib,
        const std::string &dir_,
        const std::string &name)
    {
        if(!lib.rootScope())
        {
            std::cerr << "unable to generate sources from empty lib "<<std::endl;
            return false;
        }

        _wireName = "wire_"+lib.sign().toHex(8)+"_"+name;
        _bodyName = "body_"+lib.sign().toHex(8)+"_"+name;

        boost::filesystem::path dir(dir_);

        boost::system::error_code ec;
        boost::filesystem::create_directories(dir, ec);

        if(ec)
        {
            std::cerr << "unable to create directory "<<dir.string()<<": "<<ec.message()<<std::endl;
            return false;
        }

        return true;
    }

    std::string ExecutorCpp::typeName(const Type *v, int flags)
    {
        switch(v->concrete())
        {
        case TypeConcrete::null:
            break;

        case TypeConcrete::struct_:
            {
                auto vv = static_cast<const Struct *>(v);
                return typeName(vv->scope(), vv->name(), flags);
            }
        case TypeConcrete::variant:
            {
                auto vv = static_cast<const Variant *>(v);
                return typeName(vv->scope(), vv->name(), flags);
            }
        case TypeConcrete::alias:
            {
                auto vv = static_cast<const Alias *>(v);
                return typeName(vv->scope(), vv->name(), flags);
            }
        case TypeConcrete::enum_:
            {
                auto vv = static_cast<const Enum *>(v);
                return typeName(vv->scope(), vv->name(), flags);
            }
        case TypeConcrete::errc:
            {
                auto vv = static_cast<const Errc *>(v);
                return typeName(vv->scope(), vv->name(), (flags & ~inBody) | inTarget);
            }
        case TypeConcrete::interface:
            {
                auto vv = static_cast<const Interface *>(v);
                return typeName(vv->scope(), vv->name(), flags);
            }

        case TypeConcrete::array:
            {
                auto vc = static_cast<const Array *>(v);
                return _runtimeNamespace+"::array< " + typeName(vc->elementType(), flags) + ", " + std::to_string(vc->size()) + ">";
            }
        case TypeConcrete::list:
            {
                auto vc = static_cast<const List *>(v);
                return _runtimeNamespace+"::list< " + typeName(vc->elementType(), flags) + ">";
            }
        case TypeConcrete::ptr:
            {
                auto vc = static_cast<const Ptr *>(v);
                return _runtimeNamespace+"::ptr< " + typeName(vc->elementType(), flags) + ">";
            }
        case TypeConcrete::set:
            {
                auto vc = static_cast<const Set *>(v);
                return _runtimeNamespace+"::set< " + typeName(vc->elementType(), flags) + ">";
            }
        case TypeConcrete::map:
            {
                auto vc = static_cast<const Map *>(v);
                return _runtimeNamespace+"::map< " + typeName(vc->elementType1(), flags) + ", " + typeName(vc->elementType2(), flags) + ">";
            }
        case TypeConcrete::primitive:
            {
                auto vc = static_cast<const Primitive *>(v);
                switch(vc->kind())
                {
                case PrimitiveKind::void_:  return _runtimeNamespace+"::void_";

                case PrimitiveKind::bool_:  return _runtimeNamespace+"::bool_";

                case PrimitiveKind::string: return _runtimeNamespace+"::string";

                case PrimitiveKind::int8:   return _runtimeNamespace+"::int8";
                case PrimitiveKind::int16:  return _runtimeNamespace+"::int16";
                case PrimitiveKind::int32:  return _runtimeNamespace+"::int32";
                case PrimitiveKind::int64:  return _runtimeNamespace+"::int64";

                case PrimitiveKind::uint8:  return _runtimeNamespace+"::uint8";
                case PrimitiveKind::uint16: return _runtimeNamespace+"::uint16";
                case PrimitiveKind::uint32: return _runtimeNamespace+"::uint32";
                case PrimitiveKind::uint64: return _runtimeNamespace+"::uint64";

                case PrimitiveKind::real32: return _runtimeNamespace+"::uint32";
                case PrimitiveKind::real64: return _runtimeNamespace+"::uint64";

                case PrimitiveKind::bytes:  return _runtimeNamespace+"::Bytes";

                case PrimitiveKind::interface:return _runtimeNamespace+"::Interface";
                case PrimitiveKind::iid:    return _runtimeNamespace+"::Iid";
                }

                abort();
                return "";
            }
        }

        abort();
        return "";
    }

    std::string ExecutorCpp::typeName(const Scope *v, const std::string &name, int flags)
    {
        std::string scopedName = name;
        while(v)
        {
            scopedName = v->name()+"::"+scopedName;
            v = v->scope();
        }

        if(flags&inTarget)
        {
            return scopedName;
        }

        return
                std::string(flags&ignoreTemplateTypename ? "" : "typename ")+
                (flags&forGlobalScope ? _runtimeNamespace+"::generated::" : "")+
                (flags&inWire ? _wireName : _bodyName)+
                "<"+(flags&instantiated ? "0" : "i")+">"+scopedName;
    }

    std::string ExecutorCpp::methodArguments(const Method *m, bool forRealMethod, int typesFlags)
    {
        std::string res;

        bool first = true;
        for(const Attribute *a : m->query())
        {
            if(first) first = false;
            else res+= ", ";

            res+= typeName(a->type(), typesFlags);
            if(forRealMethod)
            {
                res+= " &&";
                res+= a->name();
            }
        }

        return res;
    }

    std::string ExecutorCpp::methodArgumentsMove(const dci::couple::meta::Method *m)
    {
        std::string res;

        bool first = true;
        for(const Attribute *a : m->query())
        {
            if(first) first = false;
            else res+= ", ";

            res+= "::std::move(";
            res+= a->name();
            res+= ")";
        }

        return res;
    }

    std::string ExecutorCpp::methodReplyTypes(const dci::couple::meta::Method *m, int typesFlags)
    {
        std::string res;

        bool first = true;
        for(const Type *t : m->reply())
        {
            if(first) first = false;
            else res+= ", ";

            res+= typeName(t, typesFlags);
        }

        return res;
    }

    std::string ExecutorCpp::methodName(const dci::couple::meta::Method *m, int typesFlags, const dci::couple::meta::Interface *i)
    {
        std::string res;
        if(i) res += typeName(i, typesFlags) + "::";
        res+= m->name();
        return res;
    }

    std::string ExecutorCpp::methodSignature(const Method *m, int typesFlags, const Interface *i)
    {
        std::string res;

        if(m->noreply())
        {
            res+= _runtimeNamespace+"::void_ ";
        }
        else
        {
            res+= _runtimeNamespace+"::Future< " + methodReplyTypes(m, typesFlags) + "> ";
        }

        if(i) res += typeName(i, typesFlags) + "::";

        res+= m->name()+"("+methodArguments(m, true, typesFlags)+")";

        return res;
    }

    const std::string &ExecutorCpp::wireName() const
    {
        return _wireName;
    }

    const std::string &ExecutorCpp::bodyName() const
    {
        return _bodyName;
    }

    const std::string &ExecutorCpp::runtimeNamespace() const
    {
        return _runtimeNamespace;
    }

    const std::string &ExecutorCpp::serializeNamespace() const
    {
        return _serializeNamespace;
    }

    std::string ExecutorCpp::signInitializer(const runtime::Sign &sign)
    {
        std::string res = "{{";

        auto hex = sign.toHex();
        for(std::size_t i(0); i<16; ++i)
        {
            if(i)
            {
                res += ",";
            }
            res += std::string("0x") + hex[i*2] + hex[i*2+1];
        }

        res += "}}";

        return res;
    }

    std::vector<const Interface *> ExecutorCpp::interfaceWithAllBases(const dci::couple::meta::Interface *v)
    {
        std::vector<const Interface *> res;
        interfaceWithAllBases(res, v);
        return res;
    }

    void ExecutorCpp::interfaceWithAllBases(std::vector<const Interface *> &res, const dci::couple::meta::Interface *v)
    {
        if(res.end() == std::find(res.begin(), res.end(), v))
        {
            res.push_back(v);
        }

        for(const dci::couple::meta::Interface *b : v->bases())
        {
            interfaceWithAllBases(res, b);
        }
    }


}}}}
