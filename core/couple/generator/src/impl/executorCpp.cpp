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
        case TypeConcrete::iface:
            {
                auto vv = static_cast<const Iface *>(v);
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
                (flags&forGlobalScope ? "::dci::couple::runtime::generated::" : "")+
                (flags&inWire ? _wireName : _bodyName)+
                "<"+(flags&instantiated ? "0" : "i")+">"+scopedName;
    }

    std::string ExecutorCpp::methodArgiments(const Method *v, bool forOutput, int typesFlags)
    {
        std::string res;

        bool first = true;
        for(const Attribute *a : v->attributes())
        {
            if(first) first = false;
            else res+= ", ";

            res+= typeName(a->type(), typesFlags);
            res+= " &&";
            if(forOutput) res+= a->name();
        }

        return res;
    }

    std::string ExecutorCpp::methodSignature(const Method *m, int typesFlags, const Iface *i)
    {
        std::string res;

        res+= _runtimeNamespace+"::Future< ";

        const Type *rt = m->resultType();
        if(TypeConcrete::primitive != rt->concrete() || PrimitiveKind::void_ != static_cast<const Primitive *>(rt)->kind())
        {
            res+= typeName(rt, typesFlags);
        }
        res+= "> ";

        if(i) res += typeName(i, typesFlags);

        res+= m->name()+"("+methodArgiments(m,true, typesFlags)+")";

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

}}}}
