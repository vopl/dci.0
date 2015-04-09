#include "executorCpp.hpp"

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

    std::error_code ExecutorCpp::exec(
        const dci::couple::meta::Library &lib,
        const std::string &dir)
    {
        std::error_code ec;

/*
        WriterPredecl       predecl(dir);
        WriterDefinition    def(dir);
        WriterIfaceScope    scope(dir);
        WriterIfaceWire     wire(dir);

        WriterIndexHpp      hpp(dir);
        WriterIndexCpp      cpp(dir);

        //struct, variant
        //  definition

        for(auto v : lib.structs())
        {
            if(ec = def(v)) return ec;
            if(ec = hpp(v)) return ec;
        }

        for(auto v : lib.variants())
        {
            if(ec = def(v)) return ec;
            if(ec = hpp(v)) return ec;
        }


        //enum, alias
        //  definition

        for(auto v : lib.enums())
        {
            if(ec = def(v)) return ec;
            if(ec = hpp(v)) return ec;
        }

        for(auto v : lib.enums())
        {
            if(ec = def(v)) return ec;
            if(ec = hpp(v)) return ec;
        }


        //ifaces
        //  predecl + opposite
        //  scope
        //  wire
        //  definition + opposite

        for(auto v : lib.ifaces())
        {
            if(ec = predecl(v)) return ec;
            if(ec = scope(v)) return ec;
            if(ec = wire(v)) return ec;
            if(ec = def(v)) return ec;
            if(ec = hpp(v)) return ec;
            if(ec = cpp(v)) return ec;
        }
*/
        return std::error_code();
    }

}}}}
