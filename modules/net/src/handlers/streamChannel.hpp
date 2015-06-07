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

    private:
        void launchPumper();

    private:
        template <class T> friend struct StreamClient;
        dci::poll::Descriptor _d;

    private:
        streamChannel::Reader _reader;
        streamChannel::Writer _writer;

    private:
        bool _pumperLaunched{false};
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    StreamChannel<Address>::StreamChannel(int sock)
        : _d(sock)
    {
        launchPumper();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    StreamChannel<Address>::~StreamChannel()
    {
        close();

        while(_pumperLaunched)
        {
            dci::async::yield();
        }
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

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Future< > StreamChannel<Address>::write(Bytes &&v)
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
        Future< > res = _writer.pushRequest(std::move(v));

        if(wasEmpty && dci::poll::Descriptor::rsf_write & _d.readyState())
        {
            _writer.pump(_d);
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
    void StreamChannel<Address>::launchPumper()
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
