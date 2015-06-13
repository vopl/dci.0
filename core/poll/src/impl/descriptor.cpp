#include "descriptor.hpp"
#include <dci/poll/descriptor.hpp>
#include "engine.hpp"
#include <dci/logger.hpp>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace dci { namespace poll { namespace impl
{
    Descriptor::Descriptor(int fd)
        : _fd(fd)
    {
        if(valid())
        {
            std::error_code ec = install();
            if(ec)
            {
                LOGE("install poll descriptor: "<<ec);
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

    void Descriptor::close(bool withUninstall)
    {
        if(valid())
        {
            if(withUninstall)
            {
                uninstall();
            }
            ::close(_fd);
            _fd = -1;

            setReadyState(poll::Descriptor::rsf_error);
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

    void Descriptor::resetReadyState(std::uint_fast32_t flags)
    {
        _readyState &= ~flags;
    }

    void Descriptor::setReadyCallback(void *userData, void (*f)(void *userData, std::uint_fast32_t flags))
    {
        assert(!_readyCallback);
        _readyCallbackData = userData;
        _readyCallback = f;

        _readyEvent.reset();
        _readyState = 0;
    }

    std::error_code Descriptor::install()
    {
        assert(valid());
        assert(engine);
        if(!engine)
        {
            abort();
        }

        return engine->installDescriptor(this);
    }

    void Descriptor::uninstall()
    {
        assert(valid());
        assert(engine);
        if(!engine)
        {
            abort();
        }

        engine->uninstallDescriptor(this);
    }

    void Descriptor::setReadyState(std::uint_fast32_t flags)
    {
        if(_readyCallback)
        {
            _readyCallback(_readyCallbackData, flags);
        }
        else
        {
            _readyState |= flags;
            _readyEvent.set();
        }
    }

}}}
