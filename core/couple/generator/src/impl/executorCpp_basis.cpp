#include "executorCpp_basis.hpp"

#include <boost/filesystem.hpp>
#include <iostream>
#include <cassert>

namespace dci { namespace couple { namespace generator { namespace impl
{
    using namespace dci::couple::meta;

    ExecutorCpp_basis::ExecutorCpp_basis()
    {
    }

    ExecutorCpp_basis::~ExecutorCpp_basis()
    {
    }

    std::string ExecutorCpp_basis::name()
    {
        return "cpp_basis";
    }

    std::string ExecutorCpp_basis::description()
    {
        return "core for cpp";
    }

    bool ExecutorCpp_basis::exec(
        const Library &lib,
        const std::string &dir_,
        const std::string &name)
    {
        if(!ExecutorCpp::exec(lib, dir_, name))
        {
            return false;
        }

        try
        {
            _hpp.open((boost::filesystem::path(dir_)/(name+".hpp")).string());

            {
                _hpp<< "#pragma once"<<el;
                _hpp<< "#include <dci/couple/runtime.hpp>"<<el;
                _hpp<< el;
                _hpp<< "namespace dci { namespace couple { namespace runtime { namespace generated"<<el;
                _hpp<< "{"<<el;
                _hpp<< indent;

                _hpp<< "template <int i> struct "<<wireName()<<";"<<el;
                _hpp<< "template <int i> struct "<<bodyName()<<";"<<el;

                _hpp<< el;
                _hpp<< "template <int i>"<< el;
                _hpp<< "struct "<<wireName()<< el;
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
                _hpp<< "struct "<<bodyName()<< el;
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
    void ExecutorCpp_basis::writeTarget<Iface>(const Iface *v)
    {
        _hpp<< "using "<<v->name()<<" = "<<typeName(v, inBody|forGlobalScope|instantiated)<<";"<<el;
    }

    template <class T>
    void ExecutorCpp_basis::writeTarget(const T *v)
    {
        _hpp<< "using "<<v->name()<<" = "<<typeName(v, inBody|forGlobalScope|instantiated)<<";"<<el;
    }

    bool ExecutorCpp_basis::writeWire(const Scope *v)
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

    bool ExecutorCpp_basis::writeWire(const Struct *v)
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

    bool ExecutorCpp_basis::writeWire(const Variant *v)
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

    bool ExecutorCpp_basis::writeWire(const Iface *v)
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

            _hpp<< runtimeNamespace()+"::Wire< ";

            if(m->nowait())
            {
                _hpp<< runtimeNamespace()+"::nowaitvoid";
            }
            else
            {
                _hpp<< typeName(m->resultType(), inBody);
            }

            _hpp<< "(";

            bool first = true;
            for(auto *a : m->attributes())
            {
                if(first) first = false;
                else _hpp<< ", ";

                _hpp<< typeName(a->type(), inBody);
            }

            _hpp<< ") > "<<m->name()<<";"<<el;
        }

        _hpp<< undent;
        _hpp<< "};"<<el;

        return true;
    }

    void ExecutorCpp_basis::writeBody(const Scope *scope, bool withSelf)
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

    void ExecutorCpp_basis::writeBody(const Struct *v)
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
            _hpp<< typeName(f->type(), inBody)<<" "<<f->name()<<";"<<el;
        }

        _hpp<< undent;
        _hpp<< "};"<<el;
    }

    void ExecutorCpp_basis::writeBody(const Variant *v)
    {
        _hpp<< "// variant "<<v->name()<<el;
        _hpp<< "struct "<<v->name()<<el;

        _hpp<< indent;
        _hpp<< ": "<<runtimeNamespace()<<"::variant<"<<el;

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

    void ExecutorCpp_basis::writeBody(const Enum *v)
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

    void ExecutorCpp_basis::writeBody(const Alias *v)
    {
        _hpp<< "// alias "<<v->name()<<el;
        _hpp<< "using "<<v->name()<<" = "<<typeName(v->type(), inBody)<<";"<<el;
    }

    void ExecutorCpp_basis::writeBody(const Iface *v)
    {
        _hpp<< "// "<<"iface "<<v->name()<<el;
        _hpp<< "struct "<<v->name()<<el;

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
            _hpp<< v->name()<<"()"<<el;
            _hpp<< indent;
            _hpp<< ": Iface(new "<<typeName(v, inWire)<<", "<<(v->primary()?"true":"false")<<")"<<el;
            _hpp<< undent;
            _hpp<< "{"<<el;
            _hpp<< "}"<<el;
        }

        //move ctor
        {
            _hpp<< v->name()<<"("<<v->name()<<" &&from)"<<el;
            _hpp<< indent;
            _hpp<< ": Iface(std::forward< Iface>(from))"<<el;
            _hpp<< undent;
            _hpp<< "{"<<el;
            _hpp<< "}"<<el;
        }

        //from opposite ctor
        {
            _hpp<< v->name()<<"("<<typeName(v->opposite(), inBody)<<" &from)"<<el;
            _hpp<< indent;
            _hpp<< ": Iface(from.wire(), "<<(v->primary()?"true":"false")<<")"<<el;
            _hpp<< undent;
            _hpp<< "{"<<el;
            _hpp<< "}"<<el;
        }

        //dtor
        {
            _hpp<< "~"<<v->name()<<"()"<<el;
            _hpp<< "{"<<el;
            _hpp<< "}"<<el;
        }
        _hpp<< el;

        //move assignment operator
        {
            _hpp<< v->name()<<" &operator=("<<v->name()<<" &&from)"<<el;
            _hpp<< "{"<<el;
            _hpp<< indent;
            _hpp<< "Iface::operator=(std::forward< Iface>(from));"<<el;
            _hpp<< "return *this;"<<el;
            _hpp<< undent;
            _hpp<< "}"<<el;
        }

        //from opposite assignment
        {
            _hpp<< v->name()<<" &operator=("<<typeName(v->opposite(), inBody)<<" &from)"<<el;
            _hpp<< "{"<<el;
            _hpp<< indent;
            _hpp<< "Iface::assign(from.wire(), "<<(v->primary()?"true":"false")<<");"<<el;
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
            if(CallDirection::in == m->direction())
            {

                _hpp<< runtimeNamespace()<<"::Signal< ";
                if(m->nowait())
                {
                    _hpp<< runtimeNamespace()+"::nowaitvoid";
                }
                else
                {
                    _hpp<< typeName(m->resultType(), inBody);
                }
                _hpp<< "("<<methodArgiments(m, false, inBody)<<")> &"<<m->name()<<"()"<<el;

                _hpp<< "{"<<el;
                _hpp<< indent;
                _hpp<< "return wire()->"<<m->name()<<";"<<el;
                _hpp<< undent;
                _hpp<< "}"<<el;
            }
            else
            {
                _hpp<< methodSignature(m, inBody)<<el;

                _hpp<< "{"<<el;
                _hpp<< indent;

                _hpp<< "return wire()->"<<m->name()<<"(";

                bool first = true;
                for(const Attribute *a : m->attributes())
                {
                    if(first) first = false;
                    else _hpp<< ", ";

                    _hpp<< "std::forward< "<<typeName(a->type(), inBody)<<">("<<a->name()<<")";
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

    void ExecutorCpp_basis::writeIid(const dci::couple::meta::Scope *v)
    {
        for(auto child : v->ifaces())   writeIid(child);
        for(auto child : v->structs())  writeIid(child);
        for(auto child : v->variants()) writeIid(child);
        for(auto child : v->scopes())   writeIid(child);
    }

    void ExecutorCpp_basis::writeIid(const Iface *v)
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
    }

    void ExecutorCpp_basis::writeTarget(const Scope *scope)
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

}}}}
