#include "executorCpp_handlerSkel.hpp"

#include <boost/filesystem.hpp>
#include <iostream>
#include <cassert>

namespace dci { namespace couple { namespace generator { namespace impl
{
    using namespace dci::couple::meta;

    ExecutorCpp_handlerSkel::ExecutorCpp_handlerSkel()
    {
    }

    ExecutorCpp_handlerSkel::~ExecutorCpp_handlerSkel()
    {
    }

    std::string ExecutorCpp_handlerSkel::name()
    {
        return "cpp_handler_skel";
    }

    std::string ExecutorCpp_handlerSkel::description()
    {
        return "handler skeletons for cpp";
    }

    bool ExecutorCpp_handlerSkel::exec(
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
            _hpp.open((boost::filesystem::path(dir_)/(name+"HandlerSkel.hpp")).string());

            {
                _hpp<< "#pragma once"<<el;
                _hpp<< "#include <dci/couple/runtime.hpp>"<<el;
                _hpp<< el;
                _hpp<< "namespace dci { namespace couple { namespace runtime { namespace generated { namespace handlers"<<el;
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

    bool ExecutorCpp_handlerSkel::writeBody(const dci::couple::meta::Scope *scope, bool withSelf)
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

    bool ExecutorCpp_handlerSkel::writeBody(const dci::couple::meta::Interface *v)
    {
        _hpp<< "// "<<"handler skeleton for "<<v->name()<<el;


        _hpp<< "template <class Handler>"<<el;
        _hpp<< "struct "<<v->name()<<el;
        _hpp<< indent;
        _hpp<< ": public "<<typeName(v->opposite(), inTarget)<<el;
        _hpp<< ", public dci::mm::NewDelete<Handler>"<<el;
        _hpp<< undent;
        _hpp<< "{"<<el;
        _hpp<< indent;

        writeBody(static_cast<const Scope *>(v), false);

        _hpp<< undent;
        _hpp<< "protected:"<<el;
        _hpp<< indent;

        {
            _hpp<< "//constructor"<<el;

            _hpp<< v->name()<<"(bool deleteSelfWhenUninvolved=true)"<<el;
            _hpp<< "{"<<el;
            _hpp<< indent;
            _hpp<< "if(deleteSelfWhenUninvolved) wires()->listenUninvolve(true, [](void *userData){delete static_cast<Handler*>(userData);}, static_cast<Handler*>(this));"<<el;
            _hpp<< el;

            _hpp<< "bool b; (void)b;"<<el;
            for(const Interface *b : interfaceWithAllBases(v))
            {
                _hpp<< "//connect 'in' methods for interface "<<b->name()<<el;
                _hpp<< "{"<<el<<indent;
                for(const Method *m : b->methods())
                {
                    if(CallDirection::in == m->direction())
                    {
                        _hpp<< "b = "<<typeName(v->opposite(), inTarget)<<"::signal_"<<m->name()<<"().connect(&Handler::"<<m->name()<<", static_cast<Handler *>(this));"<<el;
                        _hpp<< "assert(b);"<<el;
                    }
                }
                _hpp<< undent<<"}"<<el;
            }
            _hpp<< undent;
            _hpp<< "}"<<el;
        }
        _hpp<< el;

        {
            _hpp<< "//destructor"<<el;

            _hpp<< "~"<<v->name()<<"()"<<el;
            _hpp<< "{"<<el;
            _hpp<< indent;

            for(const Interface *b : interfaceWithAllBases(v))
            {
                _hpp<< "//disconnect 'in' methods for interface "<<b->name()<<el;
                _hpp<< "{"<<el<<indent;
                for(const Method *m : b->methods())
                {
                    if(CallDirection::in == m->direction())
                    {
                        _hpp<< typeName(v->opposite(), inTarget)<<"::signal_"<<m->name()<<"().disconnect();"<<el;
                    }
                }
                _hpp<< undent<<"}"<<el;
            }

            _hpp<< undent;
            _hpp<< "}"<<el;
        }
        _hpp<< el;

        {
            _hpp<< "//method defenitions, '[in]' for implementation and '[out]' for use"<<el;
            for(const Interface *b : interfaceWithAllBases(v))
            {
                _hpp<< "//interface "<<b->name()<<el;
                for(const Method *m : b->methods())
                {
                    _hpp<< "// "<<(CallDirection::in == m->direction() ? "[in] " : "[out]")<<" ";
                    _hpp<< methodSignature(m, inTarget)<<";"<<el;
                }
                _hpp<< el;
            }
        }

        _hpp<< undent;
        _hpp<< "};"<<el;
        _hpp<< el;

        return true;
    }

}}}}
