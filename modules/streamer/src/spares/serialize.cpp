#include "serialize.hpp"

namespace spares
{
    StreamSource::StreamSource(dci::couple::runtime::Bytes &data, std::size_t dataSize)
        : dci::couple::serialize::Stream<Settings, StreamSource>(*this)
        , _data(data)
        , _dataSize(dataSize)
    {
    }

    void StreamSource::readBinary(void *data, std::size_t size)
    {
        assert(_data.size() >= _dataSize);
        if(_dataSize < size)
        {
            throw std::system_error(dci::couple::serialize::err_general::lowData);
        }

        _data.fillAndDropFirst((dci::couple::runtime::byte *)data, size);
        _dataSize -= size;
    }

    StreamSink::StreamSink(dci::couple::runtime::Bytes &data)
        : dci::couple::serialize::Stream<Settings, StreamSink>(*this)
        , _data(data)
    {
    }

    void StreamSink::writeBinary(const void *data, std::size_t size)
    {
        _data.append((dci::couple::runtime::byte *)data, size);
    }

}
