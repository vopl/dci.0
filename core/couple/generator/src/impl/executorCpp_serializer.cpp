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
                _hpp<< "#include <dci/couple/serialize.hpp>"<<el;
                _hpp<< el;
                _hpp<< "namespace dci { namespace couple { namespace runtime { namespace generated { namespace serializers"<<el;
                _hpp<< "{"<<el;
                _hpp<< indent;

                writeBody(lib.rootScope(), false);

                _hpp<< undent;
                _hpp<< "}}}}}"<<el;
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
        _hpp<< ": public "<<runtimeNamespace()<<"::generated::handlers"<<typeName(v->opposite(), inTarget)<<"<"<<v->name()<<"<Engine>>"<<el;
        _hpp<< ", public "<<serializeNamespace()<<"::InterfaceImplHelper<"<<v->name()<<"<Engine>, Engine>"<<el;
        _hpp<< undent;
        _hpp<< "{"<<el;
        _hpp<< indent;

        _hpp<< "using Handler = "<<runtimeNamespace()<<"::generated::handlers"<<typeName(v->opposite(), inTarget)<<"<"<<v->name()<<"<Engine>>;"<<el;
        _hpp<< "using ImplHelper = "<<serializeNamespace()<<"::InterfaceImplHelper<"<<v->name()<<"<Engine>, Engine>;"<<el;
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
        _hpp<< el;

        _hpp<< undent;
        _hpp<< "public:"<<el;
        _hpp<< indent;
        _hpp<< "//method implementations"<<el;
        {
            for(const Interface *b : interfaceWithAllBases(v))
            {
                _hpp<< "//interface "<<b->name()<<el;

                for(const Method *m : b->methods())
                {
                    if(CallDirection::in == m->direction())
                    {
                        continue;
                    }

                    _hpp<< methodSignature(m, inTarget)<<el;
                    _hpp<< "{"<<el;
                    _hpp<< indent;

                    if(m->noreply())
                    {
                        _hpp<< "struct ConcreteMessageWriter"<<el;
                        _hpp<< indent;
                            _hpp<< ": public "<<serializeNamespace()<<"::MessageWriter<OStream>"<<el;
                        _hpp<< undent;
                        _hpp<< "{"<<el;
                        _hpp<< indent;
                            _hpp<< "std::tuple< "<< methodArguments(m, false, inTarget)<<"> _args;"<<el;

                            _hpp<< "ConcreteMessageWriter("<< methodArguments(m, true, inTarget)<<")"<<el;
                            _hpp<< indent;
                                _hpp<< ": _args("<<methodArgumentsMove(m)<<")"<<el;

                                _hpp<< undent;
                                _hpp<< "{"<<el;
                                _hpp<< indent;
                                _hpp<< undent;
                                _hpp<< "}"<<el;

                                _hpp<< "virtual void write(OStream &w) override"<<el;
                                _hpp<< "{"<<el;
                                _hpp<< indent;
                                    _hpp<< serializeNamespace()<<"::MessageWriter<OStream>::write(w);"<<el;

                                    _hpp<< "w << MethodId::"<<methodIdName(b,m)<<";"<<el;
                                    _hpp<< "w << ::std::move(_args);"<<el;
                                _hpp<< undent;
                                _hpp<< "}"<<el;

                        _hpp<< undent;
                        _hpp<< "};"<<el;

                        _hpp<< "this->pushMessageWriter(std::unique_ptr<ConcreteMessageWriter>(new ConcreteMessageWriter("<<methodArgumentsMove(m)<< ")));"<<el;
                    }
                    else
                    {
                        _hpp<< "struct ConcreteRequestWriterResponseReader"<<el;
                        _hpp<< indent;
                            _hpp<< ": public "<<serializeNamespace()<<"::RequestWriterResponseReader<OStream, IStream>"<<el;
                        _hpp<< undent;
                        _hpp<< "{"<<el;
                        _hpp<< indent;
                            _hpp<< "std::tuple< "<< methodArguments(m, false, inTarget)<<"> _args;"<<el;
                            _hpp<< "::dci::couple::runtime::Promise< "<<methodReplyTypes(m, inTarget)<<"> _promise;"<<el;

                            _hpp<< "ConcreteRequestWriterResponseReader("<< methodArguments(m, true, inTarget)<<")"<<el;
                            _hpp<< indent;
                                _hpp<< ": _args("<<methodArgumentsMove(m)<<")"<<el;

                                _hpp<< undent;
                                _hpp<< "{"<<el;
                                _hpp<< indent;
                                _hpp<< undent;
                                _hpp<< "}"<<el;

                                _hpp<< "::dci::couple::runtime::Future< > result()"<<el;
                                _hpp<< "{"<<el;
                                _hpp<< indent;
                                    _hpp<< "return _promise.future();"<<el;
                                _hpp<< undent;
                                _hpp<< "}"<<el;

                                _hpp<< "virtual void write(OStream &w) override"<<el;
                                _hpp<< "{"<<el;
                                _hpp<< indent;
                                    _hpp<< ""<<serializeNamespace()<<"::RequestWriterResponseReader<OStream, IStream>::write(w);"<<el;

                                    _hpp<< "w << MethodId::"<<methodIdName(b,m)<<";"<<el;
                                    _hpp<< "w << ::std::move(_args);"<<el;
                                _hpp<< undent;
                                _hpp<< "}"<<el;

                                _hpp<< "virtual void read(IStream &r) override"<<el;
                                _hpp<< "{"<<el;
                                _hpp<< indent;
                                    _hpp<< "::std::error_code ec;"<<el;
                                    _hpp<< "switch("<<serializeNamespace()<<"::RequestWriterResponseReader<OStream, IStream>::read(r, ec))"<<el;
                                    _hpp<< "{"<<el;
                                    _hpp<< "case "<<serializeNamespace()<<"::ResponseWriter<OStream>::ReadResult::error:"<<el;
                                    _hpp<< indent;
                                        _hpp<< "_promise.resolveError(::std::move(ec));"<<el;
                                        _hpp<< "break;"<<el;
                                    _hpp<< undent;
                                    _hpp<< "case "<<serializeNamespace()<<"::ResponseWriter<OStream>::ReadResult::ok:"<<el;
                                    _hpp<< indent;
                                        _hpp<< "::std::tuple< "<<methodReplyTypes(m, inTarget)<<"> res;"<<el;
                                        _hpp<< "r >> res;"<<el;
                                        _hpp<< "_promise.resolveValue(::std::move(res));"<<el;
                                        _hpp<< "break;"<<el;
                                    _hpp<< undent;
                                    _hpp<< "}"<<el;

                                _hpp<< undent;
                            _hpp<< "}"<<el;
                        _hpp<< undent;
                        _hpp<< "};"<<el;

                        _hpp<< "return this->pushRequestWriter(std::unique_ptr<ConcreteRequestWriterResponseReader>(new ConcreteRequestWriterResponseReader("<<methodArgumentsMove(m)<< ")));"<<el;
                    }

                    _hpp<< undent;
                    _hpp<< "}"<<el;
                }
                _hpp<< el;
            }


            _hpp<< undent;
            _hpp<< "private:"<<el;
            _hpp<< indent;
            _hpp<< "//request reader"<<el;
            {
                _hpp<< "void readRequest(IStream &r, "<<serializeNamespace()<<"::CallId callId)"<<el;
                _hpp<< "{"<<el;
                _hpp<< indent;

                    _hpp<<"MethodId methodId;"<<el;
                    _hpp<<"r >> methodId;"<<el;

                    _hpp<<"switch(methodId)"<<el;
                    _hpp<< "{"<<el;

                    for(const Interface *b : interfaceWithAllBases(v))
                    {
                        _hpp<< "//interface "<<b->name()<<el;
                        for(const Method *m : b->methods())
                        {
                            if(CallDirection::out == m->direction())
                            {
                                continue;
                            }

                            if(!m->noreply())
                            {
                                _hpp<< "case MethodId::"<<methodIdName(b, m)<<":"<<el;
                                _hpp<< indent;
                                _hpp<< "{"<<el;
                                _hpp<< indent;
                                    _hpp<< "struct ConcreteResponseWriter"<<el;
                                    _hpp<< indent;
                                        _hpp<< ": public "<<serializeNamespace()<<"::ResponseWriter<OStream>"<<el;
                                    _hpp<< undent;
                                    _hpp<< "{"<<el;
                                    _hpp<< indent;
                                        _hpp<< runtimeNamespace()<<"::Future< "<< methodArguments(m, false, inTarget)<<"> _response;"<<el;

                                        _hpp<< "ConcreteResponseWriter("<<serializeNamespace()<<"::CallId callId, "<<runtimeNamespace()<<"::Future< "<< methodArguments(m, false, inTarget)<<"> &&response)"<<el;
                                        _hpp<< indent;
                                            _hpp<< ": "<<serializeNamespace()<<"::ResponseWriter<OStream>(callId)"<<el;
                                            _hpp<< ", _response(::std::move(response))"<<el;

                                            _hpp<< undent;
                                            _hpp<< "{"<<el;
                                            _hpp<< indent;
                                            _hpp<< undent;
                                            _hpp<< "}"<<el;

                                            _hpp<< "virtual void write(OStream &w) override"<<el;
                                            _hpp<< "{"<<el;
                                            _hpp<< indent;
                                                _hpp<< "if(_response.hasError())"<<el;
                                                _hpp<< "{"<<el;
                                                _hpp<< indent;
                                                    _hpp<< ""<<serializeNamespace()<<"::ResponseWriter<OStream>::write(w, _response.detachError());"<<el;
                                                _hpp<< undent;
                                                _hpp<< "}"<<el;
                                                _hpp<< "else"<<el;
                                                _hpp<< "{"<<el;
                                                _hpp<< indent;
                                                    _hpp<< ""<<serializeNamespace()<<"::ResponseWriter<OStream>::write(w);"<<el;
                                                    _hpp<< "w << ::std::move(_response.detachValue());"<<el;
                                                _hpp<< undent;
                                                _hpp<< "}"<<el;

                                            _hpp<< undent;
                                            _hpp<< "}"<<el;

                                    _hpp<< undent;
                                    _hpp<< "};"<<el;

                                    _hpp<< "::std::tuple< "<<methodArguments(m, false, inTarget)<<"> args;"<<el;
                                    _hpp<< "r >> args;"<<el;

                                    _hpp<< runtimeNamespace()<<"::Future< "<< methodArguments(m, false, inTarget)<<"> response = "<<methodName(m, inTarget, b)<<"(";
                                    for(std::size_t i(0); i<m->query().size(); ++i)
                                    {
                                        if(i) _hpp<< ", ";
                                        _hpp<< "::std::get<"<<i<<">(::std::move(args))";
                                    }
                                    _hpp<< ");"<<el;

                                    _hpp<< "this->pushMessageWriter(std::unique_ptr<ConcreteResponseWriter>(new ConcreteResponseWriter(callId, ::std::move(response))));"<<el;
                                _hpp<< undent;
                                _hpp<< "}"<<el;
                                _hpp<< "break;"<<el;
                                _hpp<< undent;
                            }
                        }
                    }

                    _hpp<< "default: "<<el;
                    _hpp<< indent;
                    _hpp<< "this->badMethodRequested(methodId);"<<el;
                    _hpp<< "break;"<<el;
                    _hpp<< undent;


                    _hpp<< "}"<<el;


                _hpp<< undent;
                _hpp<< "}"<<el;

            }









            _hpp<< undent;
            _hpp<< "private:"<<el;
            _hpp<< indent;
            _hpp<< "//message reader"<<el;
            {
                _hpp<< "void readMessage(IStream &r)"<<el;
                _hpp<< "{"<<el;
                _hpp<< indent;

                    _hpp<<"MethodId methodId;"<<el;
                    _hpp<<"r >> methodId;"<<el;

                    _hpp<<"switch(methodId)"<<el;
                    _hpp<< "{"<<el;

                    for(const Interface *b : interfaceWithAllBases(v))
                    {
                        _hpp<< "//interface "<<b->name()<<el;
                        for(const Method *m : b->methods())
                        {
                            if(CallDirection::out == m->direction())
                            {
                                continue;
                            }

                            if(m->noreply())
                            {
                                _hpp<< "case MethodId::"<<methodIdName(b, m)<<":"<<el;
                                _hpp<< indent;
                                _hpp<< "{"<<el;
                                _hpp<< indent;

                                    _hpp<< "::std::tuple< "<<methodArguments(m, false, inTarget)<<"> args;"<<el;
                                    _hpp<< "r >> args;"<<el;

                                    _hpp<< methodName(m, inTarget, b)<<"(";
                                    for(std::size_t i(0); i<m->query().size(); ++i)
                                    {
                                        if(i) _hpp<< ", ";
                                        _hpp<< "::std::get<"<<i<<">(::std::move(args))";
                                    }
                                    _hpp<< ");"<<el;

                                _hpp<< undent;
                                _hpp<< "}"<<el;
                                _hpp<< "break;"<<el;
                                _hpp<< undent;
                            }
                        }
                    }

                    _hpp<< "default: "<<el;
                    _hpp<< indent;
                    _hpp<< "this->badMethodMessaged(methodId);"<<el;
                    _hpp<< "break;"<<el;
                    _hpp<< undent;


                    _hpp<< "}"<<el;


                _hpp<< undent;
                _hpp<< "}"<<el;

            }


            _hpp<< undent;
            _hpp<< "};"<<el;

        }

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
