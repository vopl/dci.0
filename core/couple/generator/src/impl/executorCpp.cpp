#include "executorCpp.hpp"

#include <boost/filesystem.hpp>
#include <cassert>

namespace dci { namespace couple { namespace generator { namespace impl
{
    using namespace dci::couple::meta;

    ExecutorCpp::ExecutorCpp()
        //: _idlNamespace("::dci::couple::runtime::idl")
        //: _idlNamespace("::dci::couple::runtime::idl")
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
        _wireName = "wire_"+name;
        _bodyName = "body_"+name;

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
            _cpp.open((dir/(name+".cpp")).string());

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

            {
                _cpp<< "#include \""<<name<<".hpp\""<<el;

                _cpp<< el;
                _cpp<< "namespace dci { namespace couple { namespace runtime { namespace generated"<<el;
                _cpp<< "{"<<el;
                _cpp<< indent;

            }

            if(lib.rootScope())
            {
                writeWires(lib.rootScope());
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
                writeBodies(lib.rootScope());
            }

            {
                _hpp<< undent;
                _hpp<< "};"<<el;


                _hpp<< undent;
                _hpp<< "}"<<el;
                _hpp<< el;

            }
            {
                _cpp<< el;
                _cpp<< "template class "<<_wireName<<"<0>"<<el;
                _cpp<< "template class "<<_bodyName<<"<0>"<<el;

                _cpp<< undent;
                _cpp<< "}"<<el;
            }

            if(lib.rootScope())
            {
                writeTargets(lib.rootScope());
            }
        }
        catch(const std::runtime_error &e)
        {
            std::cerr<<e.what()<<std::endl;
            return false;
        }

        return true;
    }

    bool ExecutorCpp::writeWires(const Scope *v)
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
        for(auto child : v->ifaces())   res |= writeWires(child);
        for(auto child : v->structs())  res |= writeWires(child);
        for(auto child : v->variants()) res |= writeWires(child);
        for(auto child : v->scopes())   res |= writeWires(child);

        if(!v->name().empty())
        {
            _hpp<<undent;
            _hpp<< "};"<<el;
        }

        _hpp.pop(res);

        return res;
    }

    bool ExecutorCpp::writeWires(const Struct *v)
    {
        _hpp.push();

        _hpp<< "// struct "<<v->name()<<el;
        _hpp<< "struct "<<v->name()<<el;
        _hpp<< "{"<<el;
        _hpp<<indent;

        bool res = false;
        for(auto child : v->ifaces())   res |= writeWires(child);
        for(auto child : v->structs())  res |= writeWires(child);
        for(auto child : v->variants()) res |= writeWires(child);
        for(auto child : v->scopes())   res |= writeWires(child);

        _hpp<<undent;
        _hpp<< "};"<<el;

        _hpp.pop(res);

        return res;
    }

    bool ExecutorCpp::writeWires(const Variant *v)
    {
        _hpp.push();

        _hpp<< "// variant "<<v->name()<<el;
        _hpp<< "struct "<<v->name()<<el;
        _hpp<< "{"<<el;
        _hpp<<indent;

        bool res = false;
        for(auto child : v->ifaces())   res |= writeWires(child);
        for(auto child : v->structs())  res |= writeWires(child);
        for(auto child : v->variants()) res |= writeWires(child);
        for(auto child : v->scopes())   res |= writeWires(child);

        _hpp<<undent;
        _hpp<< "};"<<el;

        _hpp.pop(res);

        return res;
    }

    bool ExecutorCpp::writeWires(const Iface *v)
    {
        _hpp<< "// iface "<<v->name()<<el;
        _hpp<< "struct "<<v->name()<<el;
        _hpp<< indent;
        _hpp<< ": public IfaceWire"<<el;
        _hpp<< undent;
        _hpp<< "{"<<el;
        _hpp<< indent;

        _hpp<< v->name()<<"();"<<el;

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

    void ExecutorCpp::writeBodies(const Scope *scope)
    {
        if(!scope->name().empty())
        {
            _hpp<< "struct "<<scope->name()<<el;
            _hpp<< "{"<<el;
            _hpp<<indent;
        }

        for(auto child : scope->structs())  writeBodies(child);
        for(auto child : scope->variants()) writeBodies(child);
        for(auto child : scope->enums())    writeBodies(child);
        for(auto child : scope->aliases())  writeBodies(child);
        for(auto child : scope->ifaces())   writeBodies(child);
        for(auto child : scope->scopes())   writeBodies(child);

        if(!scope->name().empty())
        {
            _hpp<<undent;
            _hpp<< "};"<<el;
        }
    }

    void ExecutorCpp::writeBodies(const Struct *v)
    {
        _hpp<< "struct body"<<el;
        //assert(0);
    }

    void ExecutorCpp::writeBodies(const Variant *v)
    {
        _hpp<< "variant body"<<el;
        //assert(0);
    }

    void ExecutorCpp::writeBodies(const Enum *v)
    {
        _hpp<< "enum body"<<el;
        //assert(0);
    }

    void ExecutorCpp::writeBodies(const Alias *v)
    {
        _hpp<< "alias body"<<el;
        //assert(0);
    }

    void ExecutorCpp::writeBodies(const Iface *v)
    {
        _hpp<< "iface body"<<el;
        //assert(0);
    }

    void ExecutorCpp::writeTargets(const Scope *scope)
    {
        if(!scope->name().empty())
        {
            _hpp<< "namespace "<<scope->name()<<el;
            _hpp<< "{"<<el;
            _hpp<<indent;
        }

        for(auto child : scope->structs())  writeTargets(child);
        for(auto child : scope->variants()) writeTargets(child);
        for(auto child : scope->enums())    writeTargets(child);
        for(auto child : scope->aliases())  writeTargets(child);
        for(auto child : scope->ifaces())   writeTargets(child);
        for(auto child : scope->scopes())   writeTargets(child);

        if(!scope->name().empty())
        {
            _hpp<<undent;
            _hpp<< "}"<<el;
        }
    }

    void ExecutorCpp::writeTargets(const Struct *v)
    {
        _hpp<< "struct target"<<el;
        //assert(0);
    }

    void ExecutorCpp::writeTargets(const Variant *v)
    {
        _hpp<< "variant target"<<el;
        //assert(0);
    }

    void ExecutorCpp::writeTargets(const Enum *v)
    {
        _hpp<< "enum target"<<el;
        //assert(0);
    }

    void ExecutorCpp::writeTargets(const Alias *v)
    {
        _hpp<< "alias target"<<el;
        //assert(0);
    }

    void ExecutorCpp::writeTargets(const Iface *v)
    {
        _hpp<< "iface target"<<el;
        //assert(0);
    }

    std::string ExecutorCpp::typeName(const Type *v)
    {
        switch(v->concrete())
        {
        case TypeConcrete::null:
            break;

        case TypeConcrete::struct_:
            {
                auto vv = v->cast<Struct>();
                return typeNameInBody(vv->scope(), vv->name());
            }
        case TypeConcrete::variant:
            {
                auto vv = v->cast<Variant>();
                return typeNameInBody(vv->scope(), vv->name());
            }
        case TypeConcrete::alias:
            {
                auto vv = v->cast<Alias>();
                return typeNameInBody(vv->scope(), vv->name());
            }
        case TypeConcrete::enum_:
            {
                auto vv = v->cast<Enum>();
                return typeNameInBody(vv->scope(), vv->name());
            }
        case TypeConcrete::iface:
            {
                auto vv = v->cast<Iface>();
                return typeNameInBody(vv->scope(), vv->name());
            }

        case TypeConcrete::array:
            {
                auto vc = v->cast<Array>();
                return _idlNamespace+"::array< " + typeName(vc->elementType()) + ", " + std::to_string(vc->size()) + ">";
            }
        case TypeConcrete::list:
            {
                auto vc = v->cast<List>();
                return _idlNamespace+"::list< " + typeName(vc->elementType()) + ">";
            }
        case TypeConcrete::ptr:
            {
                auto vc = v->cast<Ptr>();
                return _idlNamespace+"::ptr< " + typeName(vc->elementType()) + ">";
            }
        case TypeConcrete::set:
            {
                auto vc = v->cast<Set>();
                return _idlNamespace+"::set< " + typeName(vc->elementType()) + ">";
            }
        case TypeConcrete::map:
            {
                auto vc = v->cast<Map>();
                return _idlNamespace+"::map< " + typeName(vc->elementType1()) + ", " + typeName(vc->elementType2()) + ">";
            }
        case TypeConcrete::primitive:
            {
                auto vc = v->cast<Primitive>();
                switch(vc->kind())
                {
                case PrimitiveKind::void_: return _idlNamespace+"::void_";

                case PrimitiveKind::bool_: return _idlNamespace+"::bool_";

                case PrimitiveKind::string: return _idlNamespace+"::string";

                case PrimitiveKind::int8: return _idlNamespace+"::int8";
                case PrimitiveKind::int16: return _idlNamespace+"::int16";
                case PrimitiveKind::int32: return _idlNamespace+"::int32";
                case PrimitiveKind::int64: return _idlNamespace+"::int64";

                case PrimitiveKind::uint8: return _idlNamespace+"::uint8";
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

    std::string ExecutorCpp::typeNameInBody(const dci::couple::meta::Scope *v, const std::string &name)
    {
        std::string scopedName = name;
        while(v)
        {
            scopedName = v->name()+"::"+scopedName;
            v = v->scope();
        }

        return "typename "+_bodyName+"<i>"+scopedName;
    }
}}}}
