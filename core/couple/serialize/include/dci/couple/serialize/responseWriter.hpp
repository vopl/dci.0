#pragma once

#include "writer.hpp"

namespace dci { namespace couple { namespace serialize
{

    template <class OStream>
    class ResponseWriter
        : public Writer<OStream>
    {
    protected:
        virtual void write(OStream &ostream) override = 0;
    };

}}}
