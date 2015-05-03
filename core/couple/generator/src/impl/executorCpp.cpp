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

    std::string ExecutorCpp::name()
    {
        return "cpp";
    }

    std::string ExecutorCpp::description()
    {
        return "core for cpp";
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

        try
        {
            _hpp.open((dir/(name+".hpp")).string());

            {
                _hpp<< "#pragma once"<<el;
                _hpp<< "#include <dci/couple/runtime/idl.hpp>"<<el;
                _hpp<< el;
                _hpp<< "namespace dci { namespace couple { namespace runtime { namespace generated"<<el;
                _hpp<< "{"<<el;
                _hpp<< indent;

                _hpp<< "template <int i> struct "<<_wireName<<";"<<el;
                _hpp<< "template <int i> struct "<<_bodyName<<";"<<el;

                _hpp<< el;
                _hpp<< "template <int i>"<< el;
                _hpp<< "struct "<<_wireName<< el;
                _hpp<< "{"<< el;
                _hpp<< indent;
            }

            if(lib.rootScope())
            {
                writeWire(lib.rootScope());
            }


            {
                _hpp<< undent;
                _hpp<< "};"<<el;

                _hpp<< el;
                _hpp<< "template <int i>"<< el;
                _hpp<< "struct "<<_bodyName<< el;
                _hpp<< "{"<< el;
                _hpp<< indent;
            }

            if(lib.rootScope())
            {
                writeBody(lib.rootScope(), false);
            }

            {
                _hpp<< undent;
                _hpp<< "};"<<el;

                if(lib.rootScope())
                {
                    writeIid(lib.rootScope());
                }

                _hpp<< undent;
                _hpp<< "}}}}"<<el;
                _hpp<< el;

            }

            if(lib.rootScope())
            {
                writeTarget(lib.rootScope());
            }
        }
        catch(const std::runtime_error &e)
        {
            std::cerr<<e.what()<<std::endl;
            return false;
        }

        return true;
    }

    template <>
    void ExecutorCpp::writeTarget<Iface>(const Iface *v)
    {
        _hpp<< "using "<<v->name()<<" = "<<typeName(v, inBody|forGlobalScope|instantiated)<<";"<<el;
        _hpp<< "using "<<v->name()<<"Opposite = "<<typeName(v, inBody|forGlobalScope|instantiated)<<"Opposite;"<<el;
    }

    template <class T>
    void ExecutorCpp::writeTarget(const T *v)
    {
        _hpp<< "using "<<v->name()<<" = "<<typeName(v, inBody|forGlobalScope|instantiated)<<";"<<el;
    }

    bool ExecutorCpp::writeWire(const Scope *v)
    {
        _hpp.push();

        if(!v->name().empty())
        {
            _hpp<< "// scope "<<v->name()<<el;
            _hpp<< "struct "<<v->name()<<el;
            _hpp<< "{"<<el;
            _hpp<<indent;
        }

        bool res = false;
        for(auto child : v->ifaces())   res |= writeWire(child);
        for(auto child : v->structs())  res |= writeWire(child);
        for(auto child : v->variants()) res |= writeWire(child);
        for(auto child : v->scopes())   res |= writeWire(child);

        if(!v->name().empty())
        {
            _hpp<<undent;
            _hpp<< "};"<<el;
        }

        _hpp.pop(res);

        return res;
    }

    bool ExecutorCpp::writeWire(const Struct *v)
    {
        _hpp.push();

        _hpp<< "// struct "<<v->name()<<el;
        _hpp<< "struct "<<v->name()<<el;
        _hpp<< "{"<<el;
        _hpp<<indent;

        bool res = false;
        for(auto child : v->ifaces())   res |= writeWire(child);
        for(auto child : v->structs())  res |= writeWire(child);
        for(auto child : v->variants()) res |= writeWire(child);
        for(auto child : v->scopes())   res |= writeWire(child);

        _hpp<<undent;
        _hpp<< "};"<<el;

        _hpp.pop(res);

        return res;
    }

    bool ExecutorCpp::writeWire(const Variant *v)
    {
        _hpp.push();

        _hpp<< "// variant "<<v->name()<<el;
        _hpp<< "struct "<<v->name()<<el;
        _hpp<< "{"<<el;
        _hpp<<indent;

        bool res = false;
        for(auto child : v->ifaces())   res |= writeWire(child);
        for(auto child : v->structs())  res |= writeWire(child);
        for(auto child : v->variants()) res |= writeWire(child);
        for(auto child : v->scopes())   res |= writeWire(child);

        _hpp<<undent;
        _hpp<< "};"<<el;

        _hpp.pop(res);

        return res;
    }

    bool ExecutorCpp::writeWire(const Iface *v)
    {
        _hpp<< "// iface "<<v->name()<<el;
        _hpp<< "struct "<<v->name()<<el;
        _hpp<< indent;
        _hpp<< ": public IfaceWire"<<el;
        _hpp<< undent;
        _hpp<< "{"<<el;
        _hpp<< indent;

        _hpp<< "static const Iid _iid;"<<el;

        //ctor
        {
            _hpp<< v->name()<<"()"<<el;

            _hpp<< indent;
            _hpp<< ": IfaceWire([](IfaceWire *p){delete static_cast<"<<v->name()<<"*>(p);})"<<el;
            _hpp<< undent;
            _hpp<< "{"<<el;
            _hpp<< indent;
            _hpp<< undent;
            _hpp<< "}"<<el;
        }

        for(auto *m : v->methods())
        {
            //_hpp<< "//method "<<m->name()<<el;

            _hpp<< _idlNamespace+"::Wire< ";

            if(m->nowait())
            {
                _hpp<< _idlNamespace+"::nowaitvoid";
            }
            else
            {
                _hpp<< typeName(m->resultType());
            }

            _hpp<< "(";

            bool first = true;
            for(auto *a : m->attributes())
            {
                if(first) first = false;
                else _hpp<< ", ";

                _hpp<< typeName(a->type());
            }

            _hpp<< ") > "<<m->name()<<";"<<el;
        }

        _hpp<< undent;
        _hpp<< "};"<<el;

        return true;
    }

    void ExecutorCpp::writeBody(const Scope *scope, bool withSelf)
    {
        if(withSelf)
        {
            _hpp<< "// scope "<<scope->name()<<el;
            _hpp<< "struct "<<scope->name()<<el;
            _hpp<< "{"<<el;
            _hpp<<indent;
        }

        for(auto child : scope->structs())  writeBody(child);
        for(auto child : scope->variants()) writeBody(child);
        for(auto child : scope->enums())    writeBody(child);
        for(auto child : scope->aliases())  writeBody(child);
        for(auto child : scope->ifaces())   writeBody(child);
        for(auto child : scope->scopes())   writeBody(child, true);

        if(withSelf)
        {
            _hpp<<undent;
            _hpp<< "};"<<el;
        }
    }

    void ExecutorCpp::writeBody(const Struct *v)
    {
        _hpp<< "// struct "<<v->name()<<el;
        _hpp<< "struct "<<v->name()<<el;
        if(!v->bases().empty())
        {
            _hpp<< indent;
            bool first = true;
            for(auto b : v->bases())
            {
                if(first)
                {
                    first = false;
                    _hpp<< ": ";
                }
                else
                {
                    _hpp<< ", ";
                }

                _hpp<< typeName(b, inBody|ignoreTemplateTypename)<<el;
            }

            _hpp<< undent;
        }

        _hpp<< "{"<<el;
        _hpp<< indent;

        writeBody(static_cast<const Scope *>(v), false);

        for(auto f : v->fields())
        {
            _hpp<< typeName(f->type())<<" "<<f->name()<<";"<<el;
        }

        _hpp<< undent;
        _hpp<< "};"<<el;
    }

    void ExecutorCpp::writeBody(const Variant *v)
    {
        _hpp<< "// variant "<<v->name()<<el;
        _hpp<< "struct "<<v->name()<<el;

        _hpp<< indent;
        _hpp<< ": "<<_idlNamespace<<"::variant<"<<el;

        _hpp<< indent;
        bool first = true;
        for(auto a : v->fields())
        {
            if(first)
            {
                first = false;
                _hpp<< "  ";
            }
            else
            {
                _hpp<< ", ";
            }

            _hpp<< typeName(a->type(), inBody)<<el;
        }
        _hpp<< undent;

        _hpp<< ">"<<el;
        _hpp<< undent;

        _hpp<< "{"<<el;
        _hpp<< indent;

        writeBody(static_cast<const Scope *>(v), false);

        _hpp<< undent;
        _hpp<< "};"<<el;
    }

    void ExecutorCpp::writeBody(const Enum *v)
    {
        _hpp<< "// enum "<<v->name()<<el;
        _hpp<< "enum class "<<v->name()<<el;

        _hpp<< "{"<<el;
        _hpp<< indent;

        for(auto f : v->values())
        {
            _hpp<< f->name()<<","<<el;
        }

        _hpp<< undent;
        _hpp<< "};"<<el;
    }

    void ExecutorCpp::writeBody(const Alias *v)
    {
        _hpp<< "// alias "<<v->name()<<el;
        _hpp<< "using "<<v->name()<<" = "<<typeName(v->type())<<";"<<el;
    }

    void ExecutorCpp::writeBody(const Iface *v)
    {
        writeIfaceBody(v, false);
        writeIfaceBody(v, true);
    }

    void ExecutorCpp::writeIid(const dci::couple::meta::Scope *v)
    {
        for(auto child : v->ifaces())   writeIid(child);
        for(auto child : v->structs())  writeIid(child);
        for(auto child : v->variants()) writeIid(child);
        for(auto child : v->scopes())   writeIid(child);
    }

    void ExecutorCpp::writeIid(const Iface *v)
    {
        writeIid(static_cast<const Scope *>(v));

        auto writer = [&](const std::string &target){
            _hpp<< "template <int i> const Iid "<<target<<"::_iid{{";

            auto s = v->sign().toHex();
            for(std::size_t i(0); i<16; ++i)
            {
                if(i)
                {
                    _hpp<< ",";
                }
                _hpp<< "0x"<<s[i*2]<<s[i*2+1];
            }

            _hpp<< "}};"<<el;
        };

        writer(typeName(v, inWire|ignoreTemplateTypename));
        writer(typeName(v, ignoreTemplateTypename));
        writer(typeName(v, ignoreTemplateTypename)+"Opposite");
    }

    void ExecutorCpp::writeTarget(const Scope *scope)
    {
        if(!scope->name().empty())
        {
            _hpp<< "namespace "<<scope->name()<<el;
            _hpp<< "{"<<el;
            _hpp<<indent;
        }

        for(auto child : scope->structs())  writeTarget(child);
        for(auto child : scope->variants()) writeTarget(child);
        for(auto child : scope->enums())    writeTarget(child);
        for(auto child : scope->aliases())  writeTarget(child);
        for(auto child : scope->ifaces())   writeTarget(child);
        for(auto child : scope->scopes())   writeTarget(child);

        if(!scope->name().empty())
        {
            _hpp<<undent;
            _hpp<< "}"<<el;
        }
    }

    void ExecutorCpp::writeIfaceBody(const dci::couple::meta::Iface *v, bool opposite)
    {
        std::string name = v->name() + (opposite ? "Opposite" : "");
        std::string oppositeName = typeName(v) + (opposite ? "" : "Opposite");

        _hpp<< "// "<<(opposite ? "opposite " : "")<<"iface "<<v->name()<<el;
        _hpp<< "struct "<<name<<el;

        _hpp<< indent;
        if(v->bases().empty())
        {
            _hpp<< ": public Iface"<<el;
        }
        else
        {
            bool first = true;
            for(auto b : v->bases())
            {
                if(first)
                {
                    first = false;
                    _hpp<< ": ";
                }
                else
                {
                    _hpp<< ", ";
                }

                _hpp<< typeName(b, inBody|ignoreTemplateTypename)<<el;
            }
        }
        _hpp<< undent;

        _hpp<< "{"<<el;
        _hpp<< indent;

        _hpp<< "static const Iid _iid;"<<el;

        writeBody(static_cast<const Scope *>(v), false);

        _hpp<< el;

        //ctor
        {
            _hpp<< name<<"()"<<el;
            _hpp<< indent;
            _hpp<< ": Iface(new "<<typeName(v, inWire)<<", "<<(opposite?"false":"true")<<")"<<el;
            _hpp<< undent;
            _hpp<< "{"<<el;
            _hpp<< "}"<<el;
        }

        //move ctor
        {
            _hpp<< name<<"("<<name<<" &&from)"<<el;
            _hpp<< indent;
            _hpp<< ": Iface(std::forward< Iface>(from))"<<el;
            _hpp<< undent;
            _hpp<< "{"<<el;
            _hpp<< "}"<<el;
        }

        //from opposite ctor
        {
            _hpp<< name<<"("<<oppositeName<<" &from)"<<el;
            _hpp<< indent;
            _hpp<< ": Iface(from.wire(), "<<(opposite?"false":"true")<<")"<<el;
            _hpp<< undent;
            _hpp<< "{"<<el;
            _hpp<< "}"<<el;
        }

        //dtor
        {
            _hpp<< "~"<<name<<"()"<<el;
            _hpp<< "{"<<el;
            _hpp<< "}"<<el;
        }
        _hpp<< el;

        //move assignment operator
        {
            _hpp<< name<<" &operator=("<<name<<" &&from)"<<el;
            _hpp<< "{"<<el;
            _hpp<< indent;
            _hpp<< "Iface::operator=(std::forward< Iface>(from));"<<el;
            _hpp<< "return *this;"<<el;
            _hpp<< undent;
            _hpp<< "}"<<el;
        }

        //from opposite assignment
        {
            _hpp<< name<<" &operator=("<<oppositeName<<" &from)"<<el;
            _hpp<< "{"<<el;
            _hpp<< indent;
            _hpp<< "Iface::assign(from.wire(), "<<(opposite?"false":"true")<<");"<<el;
            _hpp<< "return *this;"<<el;
            _hpp<< undent;
            _hpp<< "}"<<el;
        }
        _hpp<< el;

        //wire access
        {
            _hpp<< typeName(v, inWire)<<" *wire()"<<el;
            _hpp<< "{"<<el;
            _hpp<< indent;
            _hpp<< "return static_cast< "<<typeName(v, inWire)<<" *>(Iface::wire());"<<el;
            _hpp<< undent;
            _hpp<< "}"<<el;
        }
        _hpp<< el;

        for(const Method *m : v->methods())
        {
            bool in = CallDirection::in == m->direction();
            if(opposite) in = !in;

            if(in)
            {
                _hpp<< _idlNamespace<<"::Signal< "<<typeName(m->resultType())<<"("<<methodArgiments(m,false)<<")> &"<<m->name()<<"()"<<el;

                _hpp<< "{"<<el;
                _hpp<< indent;
                _hpp<< "return wire()->"<<m->name()<<";"<<el;
                _hpp<< undent;
                _hpp<< "}"<<el;
            }
            else
            {
                _hpp<< methodSignature(m)<<el;

                _hpp<< "{"<<el;
                _hpp<< indent;

                _hpp<< "return wire()->"<<m->name()<<"(";

                bool first = true;
                for(const Attribute *a : m->attributes())
                {
                    if(first) first = false;
                    else _hpp<< ", ";

                    _hpp<< "std::forward< "<<_idlNamespace+"::ValuePorter< "<<typeName(a->type())<<">>("<<a->name()<<")";
                }
                _hpp<< ");"<<el;

                _hpp<< undent;
                _hpp<< "}"<<el;

            }
        }
        _hpp<< el;

        _hpp<< undent;
        _hpp<< "};"<<el;
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
                return _idlNamespace+"::array< " + typeName(vc->elementType(), flags) + ", " + std::to_string(vc->size()) + ">";
            }
        case TypeConcrete::list:
            {
                auto vc = static_cast<const List *>(v);
                return _idlNamespace+"::list< " + typeName(vc->elementType(), flags) + ">";
            }
        case TypeConcrete::ptr:
            {
                auto vc = static_cast<const Ptr *>(v);
                return _idlNamespace+"::ptr< " + typeName(vc->elementType(), flags) + ">";
            }
        case TypeConcrete::set:
            {
                auto vc = static_cast<const Set *>(v);
                return _idlNamespace+"::set< " + typeName(vc->elementType(), flags) + ">";
            }
        case TypeConcrete::map:
            {
                auto vc = static_cast<const Map *>(v);
                return _idlNamespace+"::map< " + typeName(vc->elementType1(), flags) + ", " + typeName(vc->elementType2(), flags) + ">";
            }
        case TypeConcrete::primitive:
            {
                auto vc = static_cast<const Primitive *>(v);
                switch(vc->kind())
                {
                case PrimitiveKind::void_:  return _idlNamespace+"::void_";

                case PrimitiveKind::bool_:  return _idlNamespace+"::bool_";

                case PrimitiveKind::string: return _idlNamespace+"::string";

                case PrimitiveKind::int8:   return _idlNamespace+"::int8";
                case PrimitiveKind::int16:  return _idlNamespace+"::int16";
                case PrimitiveKind::int32:  return _idlNamespace+"::int32";
                case PrimitiveKind::int64:  return _idlNamespace+"::int64";

                case PrimitiveKind::uint8:  return _idlNamespace+"::uint8";
                case PrimitiveKind::uint16: return _idlNamespace+"::uint16";
                case PrimitiveKind::uint32: return _idlNamespace+"::uint32";
                case PrimitiveKind::uint64: return _idlNamespace+"::uint64";

                case PrimitiveKind::real32: return _idlNamespace+"::uint32";
                case PrimitiveKind::real64: return _idlNamespace+"::uint64";
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

        return
                std::string(flags&ignoreTemplateTypename ? "" : "typename ")+
                (flags&forGlobalScope ? "::dci::couple::runtime::generated::" : "")+
                (flags&inWire ? _wireName : _bodyName)+
                "<"+(flags&instantiated ? "0" : "i")+">"+scopedName;
    }

    std::string ExecutorCpp::methodArgiments(const Method *v, bool forOutput)
    {
        std::string res;

        bool first = true;
        for(const Attribute *a : v->attributes())
        {
            if(first) first = false;
            else res+= ", ";

            if(forOutput) res+= _idlNamespace+"::ValuePorter< ";
            res+= typeName(a->type());
            if(forOutput) res+= "> &&";
            if(forOutput) res+= a->name();
        }

        return res;
    }

    std::string ExecutorCpp::methodSignature(const Method *m, const Iface *i)
    {
        std::string res;

        res+= _idlNamespace+"::Future< ";

        const Type *rt = m->resultType();
        if(TypeConcrete::primitive != rt->concrete() || PrimitiveKind::void_ != static_cast<const Primitive *>(rt)->kind())
        {
            res+= typeName(rt);
        }
        res+= "> ";

        if(i) res += typeName(i);

        res+= m->name()+"("+methodArgiments(m,true)+")";

        return res;
    }

}}}}
