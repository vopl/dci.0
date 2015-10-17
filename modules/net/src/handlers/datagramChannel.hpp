#pragma once

#include <dci/poll.hpp>

#include "scopeSelector.hpp"
#include "../utils/sockaddr.hpp"

#include "datagramChannel/reader.hpp"
#include "datagramChannel/writer.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace net;

    template <class Address>
    struct DatagramChannel
        : ScopeSelector<Address>::datagram::template ChannelHandlerSkel<DatagramChannel<Address>>
    {
        DatagramChannel();
        ~DatagramChannel();

        Future< > bind(Address &&v);
        Future< Bytes> receiveData();
        Future< Bytes, Address> receive();
        Future< > send(Bytes &&d, Address &&a);
        Future< > close();

    private:
        dci::poll::Descriptor _d;

    private:
        std::uint_fast32_t _readyFlags {0};
        datagramChannel::Reader<Address> _reader;
        datagramChannel::Writer<Address> _writer;
    };



    template <class Address>
    DatagramChannel<Address>::DatagramChannel()
        : _d(::socket(utils::AddressSpares<Address>::af, SOCK_DGRAM|SOCK_NONBLOCK, 0))
    {
        _readyFlags = _d.readyState();
        _d.setReadyCallback(this, [](void *userData, std::uint_fast32_t flags)
        {
            DatagramChannel *self = static_cast<DatagramChannel *>(userData);
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

    template <class Address>
    DatagramChannel<Address>::~DatagramChannel()
    {
        close();
    }

    template <class Address>
    Future< > DatagramChannel<Address>::bind(Address &&v)
    {
        typename utils::AddressSpares<Address>::SockAddr sa;
        utils::fillSockaddr(v, sa);

        if(::bind(_d, reinterpret_cast<sockaddr *>(&sa), sizeof(sa)))
        {
            return systemError();
        }

        return Future<>();
    }

    template <class Address>
    Future< Bytes> DatagramChannel<Address>::receiveData()
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

    template <class Address>
    Future< Bytes, Address> DatagramChannel<Address>::receive()
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
        Future< Bytes, Address> res = _reader.pushRequest2();

        if(wasEmpty && dci::poll::Descriptor::rsf_read & _readyFlags)
        {
            _reader.pump(_d, _readyFlags);
        }

        return res;
    }

    template <class Address>
    Future< > DatagramChannel<Address>::send(Bytes &&d, Address &&a)
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
        Future< > res = _writer.pushRequest2(std::move(d), std::move(a));

        if(wasEmpty && dci::poll::Descriptor::rsf_write & _readyFlags)
        {
            _writer.pump(_d, _readyFlags);
        }

        return res;
    }

    template <class Address>
    Future< > DatagramChannel<Address>::close()
    {
        _d.close();
        _reader.close(err_system::connection_reset);
        _writer.close(err_system::connection_reset);
        return Future<>();
    }

}
