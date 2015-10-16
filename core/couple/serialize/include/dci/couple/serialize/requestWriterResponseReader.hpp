#pragma once

#include "writer.hpp"
#include <system_error>

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

        enum class ReadResult
        {
            ok,
            error,
        };

        ReadResult read(IStream &istream, std::error_code &ec);


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
        ostream << ContentType::request;
        ostream << _callId;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class OStream, class IStream>
    typename RequestWriterResponseReader<OStream, IStream>::ReadResult RequestWriterResponseReader<OStream, IStream>::read(IStream &istream, std::error_code &ec)
    {
        (void)istream;
        (void)ec;
        // TODO: read type (error or value), if error - read error and return error-status
    }

}}}
