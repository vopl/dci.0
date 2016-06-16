#pragma once

#include <dci/poll.hpp>

#include "scopeSelector.hpp"
#include "streamChannel.hpp"

#include "../utils/sockaddr.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

namespace handlers
{
    using namespace dci::couple::runtime;
    using namespace net;

    template <class Address>
    struct StreamServer
        : ScopeSelector<Address>::stream::template ServerHandlerSkel<StreamServer<Address>>
    {
        using Channel = typename ScopeSelector<Address>::stream::Channel;

        StreamServer();
        ~StreamServer();

        Future< > bind(Address &&v);
        Future< > listen();
        Future< Channel> accept();
        Future< > close();

    private:
        dci::poll::Descriptor _d;
        std::size_t _spawnedTasks{0};
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    StreamServer<Address>::StreamServer()
        : _d(::socket(utils::AddressSpares<Address>::af, SOCK_STREAM|SOCK_NONBLOCK, 0))
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    StreamServer<Address>::~StreamServer()
    {
        close();
        while(_spawnedTasks)
        {
            dci::async::yield();
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Future< > StreamServer<Address>::bind(Address &&v)
    {
        typename utils::AddressSpares<Address>::SockAddr sa;
        utils::fillSockaddr(v, sa);

        int yes = 1;
        if(setsockopt(_d, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)))
        {
            return systemError();
        }

        if(::bind(_d, (sockaddr *)&sa, sizeof(sa)))
        {
            return systemError();
        }

        return Future<>();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Future< > StreamServer<Address>::listen()
    {
        if(::listen(_d, 5))
        {
            return systemError();
        }

        return Future<>();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Future< typename StreamServer<Address>::Channel> StreamServer<Address>::accept()
    {
        auto acceptor = [](dci::poll::Descriptor &d, typename Future<Channel>::Promise &p)
        {
            if(dci::poll::Descriptor::rsf_error & d.readyState())
            {
                p.resolveError(systemError(d.error()));
                return;
            }

            if(dci::poll::Descriptor::rsf_read & d.readyState())
            {
                int sock = ::accept(d, nullptr, 0);
                if(-1 == sock)
                {
                    p.resolveError(systemError());
                    return;
                }

                auto *v = new StreamChannel<Address>(sock);
                v->start();
                p.resolveValue(Channel(*v));
                return;
            }

            assert(0);
            p.resolveError(err_system::unknown);
        };

        if(_d.readyEvent().canAcquire())
        {
            typename Future<Channel>::Promise p;
            acceptor(_d, p);
            return p.future();
        }

        _spawnedTasks++;
        return dci::async::spawn([&](typename Future<Channel>::Promise &&p){
            _d.readyEvent().acquire();
            acceptor(_d, p);
            _spawnedTasks--;
        });
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Address>
    Future< > StreamServer<Address>::close()
    {
        _d.close();
        return Future<>();
    }

}
