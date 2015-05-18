#include "epoll.hpp"
#include <dci/poller/descriptor.hpp>
#include <dci/poller/error.hpp>
#include <unistd.h>
#include <sys/epoll.h>

namespace dci { namespace poller { namespace impl
{
    Epoll::Epoll()
    {
    }

    Epoll::~Epoll()
    {
        if(-1 != _fd)
        {
            close(_fd);
            _fd = -1;
        }
    }

    std::error_code Epoll::startup()
    {
        assert(-1 == _fd);
        _fd = epoll_create(1);
        if(-1 == _fd)
        {
            return std::error_code(errno, std::system_category());
        }

        return std::error_code();
    }

    std::error_code Epoll::execute(std::int32_t timeoutms)
    {
        epoll_event eventsBuffer[128];
        int eventsAmount = epoll_wait(_fd, eventsBuffer, sizeof(eventsBuffer)/sizeof(eventsBuffer[0]), timeoutms);

        if(-1 == eventsAmount)
        {
            if(-1 == _fd)
            {
                return std::error_code();
            }

            if(EINTR == errno)
            {
                return std::error_code();
            }
            else
            {
                return std::error_code(errno, std::system_category());
            }
        }

        for(int idx(0); idx<eventsAmount; ++idx)
        {
            epoll_event &evt = eventsBuffer[idx];

            Descriptor *d = static_cast<Descriptor *>(evt.data.ptr);

            std::uint_fast32_t state =
                    ((evt.events & (EPOLLIN|EPOLLPRI)) ? (std::uint_fast32_t)poller::Descriptor::rsf_read : 0) |
                    ((evt.events & (EPOLLOUT)) ? (std::uint_fast32_t)poller::Descriptor::rsf_write : 0) |
                    ((evt.events & (EPOLLERR|EPOLLHUP)) ? (std::uint_fast32_t)poller::Descriptor::rsf_error : 0) |
                    0;

            addReadyState(d, state);
        }

        return std::error_code();
    }

    std::error_code Epoll::interrupt()
    {
        if(-1 == _fd)
        {
            return make_error_code(error::general::not_initialized);
        }

        close(_fd);
        _fd = -1;

        return std::error_code();
    }

    std::error_code Epoll::installDescriptor(Descriptor *d)
    {
        epoll_event evt{0,{0}};
        evt.data.ptr = d;
        evt.events = EPOLLIN | EPOLLOUT | EPOLLET;

        if(epoll_ctl(_fd, EPOLL_CTL_ADD, d->fd(), &evt))
        {
            return std::error_code(errno, std::system_category());
        }

        Engine::registerDescriptor(d);

        return std::error_code();
    }

    std::error_code Epoll::uninstallDescriptor(Descriptor *d)
    {
        if(!Engine::deregisterDescriptor(d))
        {
            return make_error_code(error::general::bad_descriptor);
        }

        if(epoll_ctl(_fd, EPOLL_CTL_DEL, d->fd(), nullptr))
        {
            return std::error_code(errno, std::system_category());
        }

        return std::error_code();
    }
}}}
