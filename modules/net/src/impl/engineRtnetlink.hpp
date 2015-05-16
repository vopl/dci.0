#pragma once

#include "engine.hpp"

#include <asm/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>

namespace impl
{
    class Rtnetlink
        : public Engine
    {

    public:
        Rtnetlink(Host *host);
        ~Rtnetlink();

        virtual bool valid() const override;

    private:
        bool startup();
        bool request(uint32_t type);//links or addresses
        bool read();
        void shutdown();
        void worker();

    private:
        dci::async::Event _initialized;
        dci::async::Event _stop;
        dci::async::Event _stopped;

        int         _sock;
        sockaddr_nl _address;
        iovec       _msgiov;
        msghdr      _msg;
    };
}
