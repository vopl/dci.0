#include "executorCpp.hpp"

#include <boost/filesystem.hpp>
#include <cassert>

namespace dci { namespace couple { namespace generator { namespace impl
{

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
        const dci::couple::meta::Library &lib,
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

    void ExecutorCpp::writeWires(const dci::couple::meta::Scope *scope)
    {
        if(!scope->name().empty())
        {
            _hpp<< el;
            _hpp<< "struct "<<scope->name()<<el;
            _hpp<< "{"<<el;
            _hpp<<indent;
        }

        for(auto child : scope->ifaces())       writeWires(child);
        for(auto child : scope->scopes(true))   writeWires(child);

        if(!scope->name().empty())
        {
            _hpp<<undent;
            _hpp<< "};"<<el;
        }
    }

    void ExecutorCpp::writeWires(const dci::couple::meta::Iface *v)
    {
        _hpp<< "iface wire"<<el;
        //assert(0);
    }


    void ExecutorCpp::writeBodies(const dci::couple::meta::Scope *scope)
    {
        if(!scope->name().empty())
        {
            _hpp<< el;
            _hpp<< "struct "<<scope->name()<<el;
            _hpp<< "{"<<el;
            _hpp<<indent;
        }

        for(auto child : scope->structs())      writeBodies(child);
        for(auto child : scope->variants())     writeBodies(child);
        for(auto child : scope->enums())        writeBodies(child);
        for(auto child : scope->aliases())      writeBodies(child);
        for(auto child : scope->ifaces())       writeBodies(child);
        for(auto child : scope->scopes(true))   writeBodies(child);

        if(!scope->name().empty())
        {
            _hpp<<undent;
            _hpp<< "};"<<el;
        }
    }

    void ExecutorCpp::writeBodies(const dci::couple::meta::Struct *v)
    {
        _hpp<< "struct body"<<el;
        //assert(0);
    }

    void ExecutorCpp::writeBodies(const dci::couple::meta::Variant *v)
    {
        _hpp<< "variant body"<<el;
        //assert(0);
    }

    void ExecutorCpp::writeBodies(const dci::couple::meta::Enum *v)
    {
        _hpp<< "enum body"<<el;
        //assert(0);
    }

    void ExecutorCpp::writeBodies(const dci::couple::meta::Alias *v)
    {
        _hpp<< "alias body"<<el;
        //assert(0);
    }

    void ExecutorCpp::writeBodies(const dci::couple::meta::Iface *v)
    {
        _hpp<< "iface body"<<el;
        //assert(0);
    }

    void ExecutorCpp::writeTargets(const dci::couple::meta::Scope *scope)
    {
        if(!scope->name().empty())
        {
            _hpp<< "namespace "<<scope->name()<<el;
            _hpp<< "{"<<el;
            _hpp<<indent;
        }

        for(auto child : scope->structs())      writeTargets(child);
        for(auto child : scope->variants())     writeTargets(child);
        for(auto child : scope->enums())        writeTargets(child);
        for(auto child : scope->aliases())      writeTargets(child);
        for(auto child : scope->ifaces())       writeTargets(child);
        for(auto child : scope->scopes(true))   writeTargets(child);

        if(!scope->name().empty())
        {
            _hpp<<undent;
            _hpp<< "}"<<el;
        }
    }

    void ExecutorCpp::writeTargets(const dci::couple::meta::Struct *v)
    {
        _hpp<< "struct target"<<el;
        //assert(0);
    }

    void ExecutorCpp::writeTargets(const dci::couple::meta::Variant *v)
    {
        _hpp<< "variant target"<<el;
        //assert(0);
    }

    void ExecutorCpp::writeTargets(const dci::couple::meta::Enum *v)
    {
        _hpp<< "enum target"<<el;
        //assert(0);
    }

    void ExecutorCpp::writeTargets(const dci::couple::meta::Alias *v)
    {
        _hpp<< "alias target"<<el;
        //assert(0);
    }

    void ExecutorCpp::writeTargets(const dci::couple::meta::Iface *v)
    {
        _hpp<< "iface target"<<el;
        //assert(0);
    }


}}}}
