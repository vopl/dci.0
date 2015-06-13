#pragma once

#include "scopeSelector.hpp"
#include <dci/poll.hpp>
#include "streamChannel/reader.hpp"
#include "streamChannel/writer.hpp"

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace net;

    template <class Address>
    struct StreamChannel
        : ScopeSelector<Address>::stream::template ChannelHandlerSkel<StreamChannel<Address>>
    {
        StreamChannel(int sock);
        ~StreamChannel();

        Future< Address> self();
        Future< Address> peer();

        Future< Bytes> read();
        Future< > write(Bytes &&v);

        Future< > close();

    public:
        void start();

    private:
        template <class T> friend struct StreamClient;
        dci::poll::Descriptor _d;

    private:
        std::uint_fast32_t _readyFlags {0};
        streamChannel::Reader _reader;
        streamChannel::Writer _writer;
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    StreamChannel<Address>::StreamChannel(int sock)
        : _d(sock)
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    StreamChannel<Address>::~StreamChannel()
    {
        close();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Future< Address> StreamChannel<Address>::self()
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Future< Address> StreamChannel<Address>::peer()
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Future< Bytes> StreamChannel<Address>::read()
    {
        if(!_d.valid())
        {
            return make_error_code(err_system::bad_file_descriptor);
        }

        if(dci::poll::Descriptor::rsf_error & _readyFlags)
        {
            return systemError(_d.error());
        }

        bool wasEmpty = !_reader.hasRequests();
        Future< Bytes> res = _reader.pushRequest();

        if(wasEmpty && dci::poll::Descriptor::rsf_read & _readyFlags)
        {
            _reader.pump(_d, _readyFlags);
        }

        return res;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Future< > StreamChannel<Address>::write(Bytes &&v)
    {
        if(!_d.valid())
        {
            return make_error_code(err_system::bad_file_descriptor);
        }

        if(dci::poll::Descriptor::rsf_error & _readyFlags)
        {
            return systemError(_d.error());
        }

        bool wasEmpty = !_writer.hasRequests();
        Future< > res = _writer.pushRequest(std::move(v));

        if(wasEmpty && dci::poll::Descriptor::rsf_write & _readyFlags)
        {
            _writer.pump(_d, _readyFlags);
        }

        return res;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Future< > StreamChannel<Address>::close()
    {
        _d.close();
        _reader.close(err_system::connection_reset);
        _writer.close(err_system::connection_reset);
        return Future< >();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    void StreamChannel<Address>::start()
    {
        _readyFlags = _d.readyState();
        _d.setReadyCallback(this, [](void *userData, std::uint_fast32_t flags)
        {
            StreamChannel *self = static_cast<StreamChannel *>(userData);
            self->_readyFlags |= flags;

            if(dci::poll::Descriptor::rsf_error & self->_readyFlags)
            {
                std::error_code ec = systemError(self->_d.error());
                self->_d.close();
                self->_reader.close(ec);
                self->_writer.close(ec);
                return;
            }

            if(dci::poll::Descriptor::rsf_write & self->_readyFlags)
            {
                self->_writer.pump(self->_d, self->_readyFlags);
            }

            if(dci::poll::Descriptor::rsf_read & self->_readyFlags)
            {
                self->_reader.pump(self->_d, self->_readyFlags);
            }
        });
    }

}
