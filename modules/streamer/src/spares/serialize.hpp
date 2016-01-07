#pragma once

#include <dci/couple/serialize/stream.hpp>

namespace spares
{
    struct Settings
    {
        static const std::size_t _maxSize = 1024*1024*1;
        static const dci::couple::serialize::Endianness _endianness = dci::couple::serialize::Endianness::big;
    };

    struct StreamSource
        : public dci::couple::serialize::Stream<Settings, StreamSource>
    {
        StreamSource(dci::couple::runtime::Bytes &data, std::size_t dataSize);

        template <class T> void readPod(T &v);
        void readBinary(void *data, std::size_t size);

    private:
        dci::couple::runtime::Bytes &_data;
        std::size_t _dataSize;
    };

    struct StreamSink
        : public dci::couple::serialize::Stream<Settings, StreamSink>
    {
        StreamSink(dci::couple::runtime::Bytes &data);

        template <class T> void writePod(const T &v);
        void writeBinary(const void *data, std::size_t size);

    private:
        dci::couple::runtime::Bytes &_data;
    };













    template <class T>
    void StreamSource::readPod(T &v)
    {
        readBinary(&v, sizeof(v));
    }

    template <class T>
    void StreamSink::writePod(const T &v)
    {
        writeBinary(&v, sizeof(v));
    }

}
