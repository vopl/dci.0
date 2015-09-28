#include "executorCpp_basis.hpp"

#include <boost/filesystem.hpp>
#include <iostream>
#include <cassert>
#include <boost/algorithm/string/replace.hpp>


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

                if(lib.rootScope())
                {
                    writeTarget(lib.rootScope(), Stage::pre);
                    _hpp<< el;
                }


                _hpp<< "namespace dci { namespace couple { namespace runtime { namespace generated { namespace basis"<<el;
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
                _hpp<< "}}}}}"<<el;
                _hpp<< el;

            }

            if(lib.rootScope())
            {
                writeTarget(lib.rootScope(), Stage::post);
                writeErrcSpares(lib.rootScope());
            }
        }
        catch(const std::runtime_error &e)
        {
            std::cerr<<e.what()<<std::endl;
            return false;
        }

        return true;
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
        for(auto child : v->interfaces())   res |= writeWire(child);
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
        for(auto child : v->interfaces())   res |= writeWire(child);
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
        for(auto child : v->interfaces())   res |= writeWire(child);
        for(auto child : v->structs())  res |= writeWire(child);
        for(auto child : v->variants()) res |= writeWire(child);
        for(auto child : v->scopes())   res |= writeWire(child);

        _hpp<<undent;
        _hpp<< "};"<<el;

        _hpp.pop(res);

        return res;
    }

    bool ExecutorCpp_basis::writeWire(const Interface *v)
    {
        if(!v->primary())
        {
            //opposite interfaces use primary wires
            return true;
        }

        _hpp<< "// interface concrete wires block"<<v->name()<<el;
        _hpp<< "struct "<<v->name()<<"_concrete"<<el;
        _hpp<<indent;
        _hpp<< ": public InterfaceWire"<<el;
        _hpp<<undent;
        _hpp<< "{"<<el;
        _hpp<< indent;

        for(const Method *m : v->methods())
        {
            //_hpp<< "//method "<<m->name()<<el;

            _hpp<< runtimeNamespace()+"::Wire< ";

            if(m->noreply())
            {
                _hpp<< runtimeNamespace()+"::void_";
            }
            else
            {
                _hpp<< runtimeNamespace()+"::Future< ";
                bool first = true;
                for(auto *t : m->reply())
                {
                    if(first) first = false;
                    else _hpp<< ", ";

                    _hpp<< typeName(t, inBody);
                }
                _hpp<< ">";
            }

            _hpp<< "(";

            bool first = true;
            for(auto *a : m->query())
            {
                if(first) first = false;
                else _hpp<< ", ";

                _hpp<< typeName(a->type(), inBody);
            }

            _hpp<< ") > ";
            _hpp<< (CallDirection::in == m->direction() ? "in" : "out")<<"_";
            _hpp<< m->name()<<";"<<el;
        }

        _hpp<< undent;
        _hpp<< "};"<<el;


        _hpp<< "// interface "<<v->name()<<el;
        _hpp<< "struct "<<v->name()<<el;
        _hpp<< indent;
        _hpp<< ": public InterfaceWires"<<el;
        _hpp<< undent;
        _hpp<< "{"<<el;
        _hpp<< indent;

        _hpp<< "virtual InterfaceWire* concrete(const Iid &iid) override"<<el;
        _hpp<< "{"<<el;
        _hpp<< indent;
        for(const Interface *b : interfaceWithAllBases(v))
        {
            _hpp<< "if(iid == "<<typeName(b, inBody|ignoreTemplateTypename)<<"::_iid)"<<el;
            _hpp<< "{"<<el;
            _hpp<< indent;
            _hpp<< "return &"<<interfaceWireMemberName(b, false)<<";"<<el;
            _hpp<< undent;
            _hpp<< "}"<<el;
        }
        _hpp<< "return nullptr;"<<el;
        _hpp<< undent;
        _hpp<< "}"<<el;

        for(const Interface *b : interfaceWithAllBases(v))
        {
            _hpp<< "//wire ref for "<<b->name()<<el;
            _hpp<< interfaceWireTypeName(b)<<" "<<interfaceWireMemberName(b, false)<<";"<<el;
        }

        _hpp<< undent;
        _hpp<< "};"<<el;

        return true;
    }

    std::string ExecutorCpp_basis::interfaceWireTypeName(const dci::couple::meta::Interface *v)
    {
        if(!v->primary())
        {
            v = v->opposite();
        }
        return typeName(v, inWire) + "_concrete";
    }

    std::string ExecutorCpp_basis::interfaceWireMemberName(const dci::couple::meta::Interface *v, bool invertPrimary)
    {
        if(invertPrimary)
        {
            v = v->opposite();
        }
        std::string memberName = typeName(v, inTarget|ignoreTemplateTypename|forGlobalScope);
        boost::replace_all(memberName, "::", "_");
        return memberName;
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
        for(auto child : scope->errcs())    writeBody(child);
        for(auto child : scope->aliases())  writeBody(child);
        for(auto child : scope->interfaces())   writeBody(child);
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
            _hpp<< f->name();
            if(!f->value().empty())
            {
                _hpp<< " = "<<f->value();
            }
            _hpp<< ","<<el;
        }

        _hpp<< undent;
        _hpp<< "};"<<el;
    }

    void ExecutorCpp_basis::writeBody(const Errc *v)
    {
        (void)v;
        //nothing here
    }

    void ExecutorCpp_basis::writeBody(const Alias *v)
    {
        _hpp<< "// alias "<<v->name()<<el;
        _hpp<< "using "<<v->name()<<" = "<<typeName(v->type(), inBody)<<";"<<el;
    }

    void ExecutorCpp_basis::writeBody(const Interface *v)
    {
        _hpp<< "// "<<"interface "<<v->name()<<el;
        _hpp<< "struct "<<v->name()<<el;

        _hpp<< indent;
        _hpp<< ": public Interface"<<el;
        _hpp<< undent;

        _hpp<< "{"<<el;
        _hpp<< indent;

        _hpp<< "static const Iid _iid;"<<el;

        for(const Interface *b : interfaceWithAllBases(v))
        {
            _hpp<< interfaceWireTypeName(b)<<" *"<<interfaceWireMemberName(b, !v->primary())<<"_ptr;"<<el;
        }
        _hpp<< el;

        writeBody(static_cast<const Scope *>(v), false);

        _hpp<< el;

        std::string wiresType = typeName(v->primary() ? v : v->opposite(), inWire);

        //ctor
        {
            _hpp<< v->name()<<"()"<<el;
            _hpp<< indent;
            _hpp<< ": Interface(new "<<wiresType<<", "<<(v->primary()?"true":"false")<<")"<<el;
            for(const Interface *b : interfaceWithAllBases(v))
            {
                _hpp<< ", "<<interfaceWireMemberName(b, !v->primary())<<"_ptr(&static_cast<"<<wiresType<<"*>(wires())->"<<interfaceWireMemberName(b, !v->primary())<<")"<<el;
            }

            _hpp<< undent;
            _hpp<< "{"<<el;
            _hpp<< "}"<<el;
        }

        //move ctor
        {
            _hpp<< v->name()<<"("<<v->name()<<" &&from)"<<el;
            _hpp<< indent;
            _hpp<< ": Interface(std::forward< Interface>(from))"<<el;
            for(const Interface *b : interfaceWithAllBases(v))
            {
                _hpp<< ", "<<interfaceWireMemberName(b, !v->primary())<<"_ptr(from."<<interfaceWireMemberName(b, !v->primary())<<"_ptr)"<<el;
            }
            _hpp<< undent;
            _hpp<< "{"<<el;
            _hpp<< indent;
            for(const Interface *b : interfaceWithAllBases(v))
            {
                _hpp<< "//from."<<interfaceWireMemberName(b, !v->primary())<<"_ptr = nullptr;"<<el;
            }
            _hpp<< undent;
            _hpp<< "}"<<el;
        }

        //ctor from opposite
        {
            _hpp<< v->name()<<"("<<typeName(v->opposite(), inBody)<<" &from)"<<el;
            _hpp<< indent;
            _hpp<< ": Interface(from.wires(), "<<(v->primary()?"true":"false")<<")"<<el;
            for(const Interface *b : interfaceWithAllBases(v))
            {
                _hpp<< ", "<<interfaceWireMemberName(b, !v->primary())<<"_ptr(from."<<interfaceWireMemberName(b, !v->primary())<<"_ptr)"<<el;
            }
            _hpp<< undent;
            _hpp<< "{"<<el;
            _hpp<< "}"<<el;
        }

        //move ctor from any interface
        {
            _hpp<< v->name()<<"("<<runtimeNamespace()<<"::Interface &&from)"<<el;
            _hpp<< indent;
            _hpp<< ": Interface(NullInterfaceInitializer())"<<el;
            _hpp<< undent;
            _hpp<< "{"<<el;
            _hpp<< indent;
            _hpp<< "*this = std::forward< Interface>(from);"<<el;
            _hpp<< undent;
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

            _hpp<< "if(wires() == from.wires())"<<el;
            _hpp<< "{"<<el;
            _hpp<< indent;
            _hpp<< "// already same"<<el;
            _hpp<< "return *this;"<<el;
            _hpp<< undent;
            _hpp<< "}"<<el;

            _hpp<< "Interface::operator=(std::forward< Interface>(from));"<<el;
            for(const Interface *b : interfaceWithAllBases(v))
            {
                _hpp<< interfaceWireMemberName(b, !v->primary())<<"_ptr = from."<<interfaceWireMemberName(b, !v->primary())<<"_ptr;"<<el;
                _hpp<< "//from."<<interfaceWireMemberName(b, !v->primary())<<"_ptr = nullptr;"<<el;
            }
            _hpp<< "return *this;"<<el;
            _hpp<< undent;
            _hpp<< "}"<<el;
        }

        //assignment from opposite
        {
            _hpp<< v->name()<<" &operator=("<<typeName(v->opposite(), inBody)<<" &from)"<<el;
            _hpp<< "{"<<el;
            _hpp<< indent;
            _hpp<< "Interface::assign(from.wires(), "<<(v->primary()?"true":"false")<<");"<<el;
            for(const Interface *b : interfaceWithAllBases(v))
            {
                _hpp<< interfaceWireMemberName(b, !v->primary())<<"_ptr = from."<<interfaceWireMemberName(b, !v->primary())<<"_ptr;"<<el;
            }
            _hpp<< "return *this;"<<el;
            _hpp<< undent;
            _hpp<< "}"<<el;
        }
        _hpp<< el;


        //move assignment from any interface
        {
            _hpp<< v->name()<<"& operator=("<<runtimeNamespace()<<"::Interface &&from)"<<el;
            _hpp<< "{"<<el;
            _hpp<< indent;

            _hpp<< "if(wires() == from.wires())"<<el;
            _hpp<< "{"<<el;
            _hpp<< indent;
            _hpp<< "return *this;"<<el;
            _hpp<< undent;
            _hpp<< "}"<<el;

            for(const Interface *b : interfaceWithAllBases(v))
            {
                _hpp<< interfaceWireMemberName(b, !v->primary())<<"_ptr = static_cast<"<<interfaceWireTypeName(b)<<"*>(from.wires()->concrete("<<typeName(b, inBody|ignoreTemplateTypename)<<"::_iid));"<<el;
                _hpp<< "if(!"<<interfaceWireMemberName(b, !v->primary())<<"_ptr)"<<el;
                _hpp<< "{"<<el;
                _hpp<< indent;
                _hpp<< "return *this;"<<el;
                _hpp<< undent;
                _hpp<< "}"<<el;
            }

            _hpp<< "if("<<(v->primary()?"true":"false")<<" == from.fwd())"<<el;
            _hpp<< "{"<<el;
            _hpp<< indent;
            _hpp<< "Interface::operator=(std::forward< Interface>(from));"<<el;
            _hpp<< undent;
            _hpp<< "}"<<el;
            _hpp<< "else"<<el;
            _hpp<< "{"<<el;
            _hpp<< indent;
            _hpp<< "Interface::assign(from.wires(), "<<(v->primary()?"true":"false")<<");"<<el;
            _hpp<< undent;
            _hpp<< "}"<<el;


            _hpp<< "return *this;"<<el;
            _hpp<< undent;
            _hpp<< "}"<<el;
        }


        for(const Interface *b : interfaceWithAllBases(v))
        {
            _hpp<< "// methods from "<<b->name()<<el;

            for(const Method *m : b->methods())
            {
                if(CallDirection::out == m->direction())
                {

                    _hpp<< runtimeNamespace()<<"::Signal< ";
                    if(m->noreply())
                    {
                        _hpp<< runtimeNamespace()+"::void_";
                    }
                    else
                    {
                        _hpp<< runtimeNamespace()+"::Future< "+methodReplyTypes(m, inBody)+">";
                    }
                    _hpp<< "("<<methodArguments(m, false, inBody)<<")> &signal_"<<m->name()<<"()"<<el;

                    _hpp<< "{"<<el;
                    _hpp<< indent;
                    _hpp<< "return "<<interfaceWireMemberName(b, !v->primary())<<"_ptr->"<<(b->primary() ? "out" : "in")<<"_"<<m->name()<<";"<<el;
                    _hpp<< undent;
                    _hpp<< "}"<<el;
                }
                else
                {
                    _hpp<< methodSignature(m, inBody)<<el;

                    _hpp<< "{"<<el;
                    _hpp<< indent;

                    _hpp<< "return "<<interfaceWireMemberName(b, !v->primary())<<"_ptr->"<<(b->primary() ? "in" : "out")<<"_"<<m->name()<<"(";

                    bool first = true;
                    for(const Attribute *a : m->query())
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
        }
        _hpp<< el;

        _hpp<< undent;
        _hpp<< "};"<<el;
    }

    void ExecutorCpp_basis::writeIid(const dci::couple::meta::Scope *v)
    {
        for(auto child : v->interfaces())   writeIid(child);
        for(auto child : v->structs())  writeIid(child);
        for(auto child : v->variants()) writeIid(child);
        for(auto child : v->scopes())   writeIid(child);
    }

    void ExecutorCpp_basis::writeIid(const Interface *v)
    {
        writeIid(static_cast<const Scope *>(v));

        auto writer = [&](const std::string &target){
            _hpp<< "template <int i> const Iid "<<target<<"::_iid"<<signInitializer(v->sign())<<";"<<el;
        };

        writer(typeName(v, ignoreTemplateTypename));
    }

    template <>
    void ExecutorCpp_basis::writeTarget<Interface>(const Interface *v)
    {
        _hpp<< "using "<<v->name()<<" = "<<typeName(v, inBody|forGlobalScope|instantiated)<<";"<<el;
    }

    template <>
    void ExecutorCpp_basis::writeTarget<Errc>(const Errc *v)
    {
        _hpp<< "// errc "<<v->name()<<el;
        _hpp<< "enum class "<<v->name()<<el;

        _hpp<< "{"<<el;
        _hpp<< indent;

        int idx = 0;
        for(auto f : v->values())
        {
            _hpp<< f->name()<<(!idx++ ? "=1":"") <<",";
            if(!f->description().empty() && f->description()!=f->name())
            {
                _hpp<< " //"<<f->description();
            }
            _hpp<< el;
        }

        _hpp<< undent;
        _hpp<< "};"<<el;

        // make_error_code
        _hpp<< "inline std::error_code make_error_code("<<v->name()<<" e)"<<el;
        _hpp<< "{"<<el;
        _hpp<< indent;

        _hpp <<"static const std::error_category& category = "<<runtimeNamespace()<<"::errcCategory("<<el;
        _hpp<< indent;
        _hpp<< signInitializer(v->sign())<<","<<el;
        _hpp<< "\""<<typeName(v, inTarget)<<"\","<<el;
        _hpp <<"{"<<el;
        _hpp<< indent;

        for(auto f : v->values())
        {
            std::string descr = f->description();
            boost::replace_all(descr, "\\", "\\\\");
            boost::replace_all(descr, "\"", "\\\"");
            _hpp<< "{static_cast<int>("<<v->name()<<"::"<<f->name()<<"), \""<<descr<<"\"},"<<el;
        }

        _hpp<< undent;
        _hpp<< "});"<<el;
        _hpp<< undent;

        _hpp<< "return std::error_code(static_cast<int>(e), category);"<<el;
        _hpp<< undent;
        _hpp<< "}"<<el;
    }

    template <class T>
    void ExecutorCpp_basis::writeTarget(const T *v)
    {
        _hpp<< "using "<<v->name()<<" = "<<typeName(v, inBody|forGlobalScope|instantiated)<<";"<<el;
    }

    void ExecutorCpp_basis::writeTarget(const Scope *scope, Stage stage)
    {
        if(!scope->name().empty())
        {
            _hpp<< "namespace "<<scope->name()<<el;
            _hpp<< "{"<<el;
            _hpp<<indent;
        }

        if(Stage::pre == stage)
        {
            for(auto child : scope->errcs())    writeTarget(child);
        }
        else
        {
            for(auto child : scope->structs())  writeTarget(child);
            for(auto child : scope->variants()) writeTarget(child);
            for(auto child : scope->enums())    writeTarget(child);
            for(auto child : scope->aliases())  writeTarget(child);
            for(auto child : scope->interfaces())   writeTarget(child);
        }
        for(auto child : scope->scopes())   writeTarget(child, stage);

        if(!scope->name().empty())
        {
            _hpp<<undent;
            _hpp<< "}"<<el;
        }
    }

    void ExecutorCpp_basis::writeErrcSpares(const dci::couple::meta::Scope *scope)
    {
        for(auto child : scope->structs())  writeErrcSpares(child);
        for(auto child : scope->errcs())    writeErrcSpares(child);
        for(auto child : scope->interfaces())   writeErrcSpares(child);
        for(auto child : scope->scopes())   writeErrcSpares(child);
    }

    void ExecutorCpp_basis::writeErrcSpares(const dci::couple::meta::Errc *v)
    {
        //is_error_code_enum
        _hpp<< "namespace std { template<> struct is_error_code_enum< "<<typeName(v, inTarget)<<"> : public true_type {}; }"<<el;

        //initializer
        _hpp<< "template class "<<runtimeNamespace()<<"::ErrcInitializer< "<<typeName(v, inTarget)<<">;"<<el;
    }

}}}}
