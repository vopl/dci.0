#include "executorCpp_serializer.hpp"

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <iostream>
#include <cassert>

namespace dci { namespace couple { namespace generator { namespace impl
{
    using namespace dci::couple::meta;

    ExecutorCpp_serializer::ExecutorCpp_serializer()
    {
    }

    ExecutorCpp_serializer::~ExecutorCpp_serializer()
    {
    }

    std::string ExecutorCpp_serializer::name()
    {
        return "cpp_serializer";
    }

    std::string ExecutorCpp_serializer::description()
    {
        return "serialization handlers for cpp";
    }

    bool ExecutorCpp_serializer::exec(
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
            _hpp.open((boost::filesystem::path(dir_)/(name+"Serializer.hpp")).string());

            {
                _hpp<< "#pragma once"<<el;
                _hpp<< "#include <dci/couple/runtime.hpp>"<<el;
                _hpp<< el;
                _hpp<< "namespace dci { namespace couple { namespace runtime { namespace ser"<<el;
                _hpp<< "{"<<el;
                _hpp<< indent;

                writeBody(lib.rootScope(), false);

                _hpp<< undent;
                _hpp<< "}}}}"<<el;
                _hpp<< el;

            }
        }
        catch(const std::runtime_error &e)
        {
            std::cerr<<e.what()<<std::endl;
            return false;
        }

        return true;
    }

    bool ExecutorCpp_serializer::writeBody(const dci::couple::meta::Scope *scope, bool withSelf)
    {
        _hpp.push();

        if(withSelf)
        {
            _hpp<< "// scope "<<scope->name()<<el;
            _hpp<< "namespace "<<scope->name()<<el;
            _hpp<< "{"<<el;
            _hpp<<indent;
        }

        bool res = false;
        for(auto child : scope->structs())  res |= writeBody(child, true);
        for(auto child : scope->variants()) res |= writeBody(child, true);
        for(auto child : scope->interfaces())   res |= writeBody(child);
        for(auto child : scope->scopes())   res |= writeBody(child, true);

        if(withSelf)
        {
            _hpp<<undent;
            _hpp<< "}"<<el;
        }

        _hpp.pop(res);

        return res;
    }

    bool ExecutorCpp_serializer::writeBody(const dci::couple::meta::Interface *v)
    {
        _hpp<< "// "<<"serialize handler for "<<v->name()<<el;


        _hpp<< "template <class Engine>"<<el;
        _hpp<< "struct "<<v->name()<<el;
        _hpp<< indent;
        _hpp<< ": public ::dci::couple::runtime::hs"<<typeName(v->opposite(), inTarget)<<"<"<<v->name()<<"<Engine>>"<<el;
        _hpp<< ", public ::dci::couple::serialize::InterfaceImplHelper<"<<v->name()<<"<Engine>, Engine>"<<el;
        _hpp<< undent;
        _hpp<< "{"<<el;
        _hpp<< indent;

        _hpp<< "using Handler = ::dci::couple::runtime::hs"<<typeName(v->opposite(), inTarget)<<"<"<<v->name()<<"<Engine>>;"<<el;
        _hpp<< "using ImplHelper = ::dci::couple::serialize::InterfaceImplHelper<"<<v->name()<<"<Engine>, Engine>;"<<el;
        _hpp<< "using OStream = typename Engine::OStream;"<<el;
        _hpp<< "using IStream = typename Engine::IStream;"<<el;

        _hpp<< el;

        writeBody(static_cast<const Scope *>(v), false);

        _hpp<< undent;
        _hpp<< "public:"<<el;
        _hpp<< indent;

        _hpp<< "//constructor"<<el;
        {

            _hpp<< v->name()<<"(Engine &engine, bool deleteSelfWhenUninvolved=true)"<<el;
            _hpp<< indent;
            _hpp<< ": Handler(deleteSelfWhenUninvolved)"<<el;
            _hpp<< ", ImplHelper(engine)"<<el;
            _hpp<< undent;
            _hpp<< "{"<<el;
            _hpp<< indent;
            _hpp<< undent;
            _hpp<< "}"<<el;
        }
        _hpp<< el;

        _hpp<< "//destructor"<<el;
        {

            _hpp<< "~"<<v->name()<<"()"<<el;
            _hpp<< "{"<<el;
            _hpp<< indent;
            _hpp<< undent;
            _hpp<< "}"<<el;
        }
        _hpp<< el;

        _hpp<< "//method bodies"<<el;
        {
            for(const Interface *b : interfaceWithAllBases(v))
            {
                _hpp<< "//interface "<<b->name()<<el;
                for(const Method *m : b->methods())
                {
                    if(CallDirection::in == m->direction())
                    {
                        _hpp<< methodSignature(m, inTarget)<<" {assert(0);}"<<el;
                    }
                    else
                    {
                        _hpp<< "//[out] "<<methodSignature(m, inTarget)<<";"<<el;
                    }
                }
                _hpp<< el;
            }
        }

        _hpp<< el;
        _hpp<< undent;
        _hpp<< "private:"<<el;
        _hpp<< indent;

        _hpp<< "//method identifiers"<<el;
        {
            _hpp<< "enum class MethodId"<<el;
            _hpp<< "{"<<el;
            _hpp<< indent;

            std::size_t idx(1);

            for(const Interface *b : interfaceWithAllBases(v))
            {
                _hpp<< "//interface "<<b->name()<<el;
                //b = b->primary() ? b : b->opposite();

                for(const Method *m : b->methods())
                {
                    _hpp<< methodIdName(b, m)<<" = "<<idx<<","<<el;
                    idx++;
                }
                _hpp<< el;
            }

            _hpp<< undent;
            _hpp<< "};"<<el;

        }

        _hpp<< undent;
        _hpp<< "};"<<el;
        _hpp<< el;

        return true;
    }

    std::string ExecutorCpp_serializer::methodIdName(const dci::couple::meta::Interface *i, const dci::couple::meta::Method *m)
    {
        std::string memberName = typeName(i, inTarget);
        memberName.erase(0, 2);//remove leading "::"
        boost::replace_all(memberName, "::", "_");
        return memberName+"_"+(CallDirection::in == m->direction() ? "in" : "out")+"_"+m->name();
    }

}}}}
