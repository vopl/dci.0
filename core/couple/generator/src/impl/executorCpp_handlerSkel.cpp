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
        return "handler ckeletons for cpp";
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
                _hpp<< "namespace dci { namespace couple { namespace runtime { namespace hs"<<el;
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
        for(auto child : scope->ifaces())   res |= writeBody(child);
        for(auto child : scope->scopes())   res |= writeBody(child, true);

        if(withSelf)
        {
            _hpp<<undent;
            _hpp<< "}"<<el;
        }

        _hpp.pop(res);

        return res;
    }

    bool ExecutorCpp_handlerSkel::writeBody(const dci::couple::meta::Iface *v)
    {
        _hpp<< "// "<<"handler skeleton for "<<v->name()<<el;


        _hpp<< "template <class Service>"<<el;
        _hpp<< "struct "<<v->name()<<el;
        _hpp<< indent;
        _hpp<< ": public "<<typeName(v->opposite(), inTarget)<<el;
        _hpp<< ", public dci::mm::NewDelete<Service>"<<el;
        _hpp<< undent;
        _hpp<< "{"<<el;
        _hpp<< indent;

        writeBody(static_cast<const Scope *>(v), false);

        _hpp<< undent;
        _hpp<< "protected:"<<el;
        _hpp<< indent;

        {
            _hpp<< "//constructor"<<el;

            _hpp<< v->name()<<"()"<<el;
            _hpp<< "{"<<el;
            _hpp<< indent;
            _hpp<< "wire()->listenUninvolve(true, [](void *userData){delete static_cast<Service*>(userData);}, static_cast<Service*>(this));"<<el;

            _hpp<< "//connect"<<el;
            _hpp<< "bool b; (void)b;"<<el;
            for(const Method *m : v->methods())
            {
                if(CallDirection::out == m->direction())
                {
                    _hpp<< "b = wire()->"<<m->name()<<".connect(&Service::"<<m->name()<<", static_cast<Service *>(this));"<<el;
                    _hpp<< "assert(b);"<<el;
                }
            }
            _hpp<< undent;
            _hpp<< "};"<<el;
        }

        {
            _hpp<< "//destructor"<<el;

            _hpp<< "~"<<v->name()<<"()"<<el;
            _hpp<< "{"<<el;
            _hpp<< indent;

            _hpp<< "//disconnect"<<el;
            for(const Method *m : v->methods())
            {
                if(CallDirection::out == m->direction())
                {
                    _hpp<< "wire()->"<<m->name()<<".disconnect();"<<el;
                }
            }
            _hpp<< undent;
            _hpp<< "};"<<el;
        }

        {
            _hpp<< "//method prototypes"<<el;
            for(const Method *m : v->methods())
            {
                if(CallDirection::out == m->direction())
                {
                    _hpp<< methodSignature(m, inTarget)<<" = delete;"<<el;
                }
            }
        }

        _hpp<< undent;
        _hpp<< "};"<<el;

        return true;
    }

}}}}
