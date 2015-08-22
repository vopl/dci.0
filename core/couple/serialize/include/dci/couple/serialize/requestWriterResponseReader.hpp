#pragma once

#include "writer.hpp"

namespace dci { namespace couple { namespace serialize
{

    using CallId = std::uint64_t;

    template <class OStream, class IStream>
    class RequestWriterResponseReader
        : public Writer<OStream>
    {
    public:
        void setCallId(CallId callId);

    protected:
        virtual void write(OStream &ostream) override = 0;
        virtual void read(IStream &istream) = 0;

    private:
        CallId _callId;
    };

    template <class OStream, class IStream>
    using RequestWriterResponseReaderPtr = std::unique_ptr<RequestWriterResponseReader<OStream, IStream>>;










    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class OStream, class IStream>
    void RequestWriterResponseReader<OStream, IStream>::setCallId(CallId callId)
    {
        _callId = callId;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class OStream, class IStream>
    void RequestWriterResponseReader<OStream, IStream>::write(OStream &ostream)
    {
        ContentType contentType = ContentType::request;
//        ostream.save(std::move(contentType));
//        ostream.save(_callId);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class OStream, class IStream>
    void RequestWriterResponseReader<OStream, IStream>::read(IStream &istream)
    {
        (void)istream;
        // empty is ok
    }

}}}
