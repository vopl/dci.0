#pragma once

#include "scopeSelector.hpp"
#include "streamChannel.hpp"

#include <dci/async.hpp>
#include <dci/mm.hpp>

#include "../utils/sockaddr.hpp"

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace net;

    template <class Address>
    struct StreamClient
        : ScopeSelector<Address>::stream::template ClientHandlerSkel<StreamClient<Address>>
    {
        using Channel = typename ScopeSelector<Address>::stream::Channel;

        StreamClient();
        ~StreamClient();

        Future< > bind(Address &&v);
        Future< Channel> connect(Address &&address);
        Future< > abort();

    private:
        bool _bounded{false};
        Address _boundAddress;
        std::size_t _spawnedTasks{0};
        dci::async::Event _abortEvent{};
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    StreamClient<Address>::StreamClient()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    StreamClient<Address>::~StreamClient()
    {
        abort().wait();
        while(_spawnedTasks)
        {
            dci::async::yield();
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Future< > StreamClient<Address>::bind(Address &&v)
    {
        _bounded = true;
        _boundAddress = std::move(v);
        return Future< >();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Future< typename StreamClient<Address>::Channel> StreamClient<Address>::connect(Address &&address)
    {
        int sock = ::socket(utils::AddressSpares<Address>::af, SOCK_STREAM|SOCK_NONBLOCK, 0);
        if(_bounded)
        {
            typename utils::AddressSpares<Address>::SockAddr sa;
            utils::fillSockaddr(sa, _boundAddress);

            if(::bind(sock, (sockaddr *)&sa, sizeof(sa)))
            {
                close(sock);
                return systemError();
            }
        }

        _spawnedTasks++;
        return dci::async::spawn([&, sock](typename Future<Channel>::Promise &&p)
        {
            StreamChannel<Address> *channel = new StreamChannel<Address>(sock);

            std::error_code ec;

            {
                typename utils::AddressSpares<Address>::SockAddr sa;
                utils::fillSockaddr(sa, address);

                int res = ::connect(channel->_d, (sockaddr *)&sa, sizeof(sa));
                if(!res)
                {
                    ec = std::error_code();
                }
                else if(EINPROGRESS != errno)
                {
                    ec = systemError();
                }
                else
                {
                    if(0 == dci::async::waitAny(_abortEvent, channel->_d.readyEvent()))
                    {
                        channel->_d.close();
                        ec = err_system::connection_aborted;
                    }
                    else if(dci::poll::Descriptor::rsf_error & channel->_d.readyState())
                    {
                        ec = systemError(channel->_d.error());
                    }
                    else if(dci::poll::Descriptor::rsf_write & channel->_d.readyState())
                    {
                        ec = std::error_code();
                    }
                    else
                    {
                        assert(!"impossible");
                        channel->_d.close();
                        ec = err_system::unknown;
                    }
                }
            }

            if(ec)
            {
                delete channel;
                p.resolveError(std::move(ec));
            }
            else
            {
                p.resolveValue(Channel(*channel));
            }

            _spawnedTasks--;
        });
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Future< > StreamClient<Address>::abort()
    {
        return dci::async::spawn([&](typename Future<>::Promise &&p)
        {
            _abortEvent.set();
            while(_spawnedTasks)
            {
                dci::async::yield();
            }

            p.resolveValue();
        });
    }

}
