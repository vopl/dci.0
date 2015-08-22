#pragma once

#include <memory>

namespace dci { namespace couple { namespace serialize
{
    enum class ContentType
    {
        request,
        response,
        message,
    };

    template <class OStream>
    class Writer
    {
        virtual void write(OStream &ostream) = 0;
    };

    template <class OStream>
    using WriterPtr = std::unique_ptr<Writer<OStream>>;

}}}
