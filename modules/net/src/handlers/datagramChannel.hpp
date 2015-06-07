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
        void launchPumper();

    private:
        dci::poll::Descriptor _d;

    private:
        datagramChannel::Reader<Address> _reader;
        datagramChannel::Writer<Address> _writer;

    private:
        bool _pumperLaunched{false};
    };



    template <class Address>
    DatagramChannel<Address>::DatagramChannel()
        : _d(::socket(utils::AddressSpares<Address>::af, SOCK_DGRAM|SOCK_NONBLOCK, 0))
    {
        launchPumper();
    }

    template <class Address>
    DatagramChannel<Address>::~DatagramChannel()
    {
        close();

        while(_pumperLaunched)
        {
            dci::async::yield();
        }
    }

    template <class Address>
    Future< > DatagramChannel<Address>::bind(Address &&v)
    {
        typename utils::AddressSpares<Address>::SockAddr sa;
        utils::fillSockaddr(v, sa);

        if(::bind(_d, (sockaddr *)&sa, sizeof(sa)))
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

        if(dci::poll::Descriptor::rsf_error & _d.readyState())
        {
            return systemError(_d.error());
        }

        bool wasEmpty = !_reader.hasRequests();
        Future< Bytes> res = _reader.pushRequest();

        if(wasEmpty && dci::poll::Descriptor::rsf_read & _d.readyState())
        {
            _reader.pump(_d);
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

        if(dci::poll::Descriptor::rsf_error & _d.readyState())
        {
            return systemError(_d.error());
        }

        bool wasEmpty = !_reader.hasRequests();
        Future< Bytes, Address> res = _reader.pushRequest2();

        if(wasEmpty && dci::poll::Descriptor::rsf_read & _d.readyState())
        {
            _reader.pump(_d);
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

        if(dci::poll::Descriptor::rsf_error & _d.readyState())
        {
            return systemError(_d.error());
        }

        bool wasEmpty = !_writer.hasRequests();
        Future< > res = _writer.pushRequest2(std::move(d), std::move(a));

        if(wasEmpty && dci::poll::Descriptor::rsf_write & _d.readyState())
        {
            _writer.pump(_d);
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

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    void DatagramChannel<Address>::launchPumper()
    {
        assert(!_pumperLaunched);

        _pumperLaunched = true;
        dci::async::spawn([&]()
        {
            while(_d.valid() && !(dci::poll::Descriptor::rsf_error & _d.readyState()))
            {
                _d.readyEvent().acquire();
                if(!_d.valid())
                {
                    break;
                }

                if(dci::poll::Descriptor::rsf_error & _d.readyState())
                {
                    std::error_code ec = systemError(_d.error());
                    _reader.close(ec);
                    _writer.close(ec);
                    break;
                }

                if(dci::poll::Descriptor::rsf_write & _d.readyState())
                {
                    _writer.pump(_d);
                }

                if(dci::poll::Descriptor::rsf_read & _d.readyState())
                {
                    _reader.pump(_d);
                }

                _d.readyEvent().reset();
            }

            _pumperLaunched = false;
        });
    }

}
