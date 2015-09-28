#pragma once

#include "writer.hpp"

namespace dci { namespace couple { namespace serialize
{

    template <class OStream>
    class MessageWriter
        : public Writer<OStream>
    {
    protected:
        virtual void write(OStream &ostream) override = 0;
    };

    template <class OStream>
    void MessageWriter<OStream>::write(OStream &ostream)
    {
        (void)ostream;
        //empty ok
    }

}}}
