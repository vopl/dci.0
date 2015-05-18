#include "descriptor.hpp"
#include "engine.hpp"
#include <dci/logger.hpp>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace dci { namespace poller { namespace impl
{
    Descriptor::Descriptor(int fd)
        : _fd(fd)
    {
        if(valid())
        {
            std::error_code ec = install();
            if(ec)
            {
                LOGE("install poller descriptor: "<<ec);
                close();
            }
        }
    }

    Descriptor::~Descriptor()
    {
        close();
    }

    bool Descriptor::valid() const
    {
        return 0 <= _fd;
    }

    std::error_code Descriptor::error()
    {
        int errcode = ENOTSOCK;
        socklen_t errcodelen = sizeof(errcode);
        if(-1 == getsockopt(_fd, SOL_SOCKET, SO_ERROR, &errcode, &errcodelen))
        {
            errcode = errno;
        }

        return std::error_code(errcode, std::system_category());
    }

    int Descriptor::fd() const
    {
        return _fd;
    }

    void Descriptor::close()
    {
        if(valid())
        {
            uninstall();
            ::close(_fd);
            _fd = -1;
        }
    }

    void Descriptor::detach()
    {
        if(valid())
        {
            uninstall();
            _fd = -1;
        }
    }

    dci::async::Event &Descriptor::readyEvent()
    {
        return _readyEvent;
    }

    std::uint_fast32_t Descriptor::readyState() const
    {
        return _readyState;
    }

    void Descriptor::resetReadyState()
    {
        _readyState = 0;
        _readyEvent.reset();
    }

    std::uint_fast32_t Descriptor::seizeReadyState()
    {
        std::uint32_t res = readyState();
        resetReadyState();
        return res;

    }

    std::error_code Descriptor::install()
    {
        assert(valid());
        if(engine)
        {
            return engine->installDescriptor(this);
        }

        return std::make_error_code(std::errc::no_such_device);
    }

    void Descriptor::uninstall()
    {
        if(engine)
        {
            engine->uninstallDescriptor(this);
        }
    }

    void Descriptor::addReadyState(std::uint_fast32_t state)
    {
        _readyState |= state;
        _readyEvent.set();
    }

}}}
