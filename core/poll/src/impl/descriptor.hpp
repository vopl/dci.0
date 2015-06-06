#pragma once

#include <dci/async.hpp>
#include <system_error>

namespace dci { namespace poll { namespace impl
{
    class Engine;

    class Descriptor
    {
    public:
        Descriptor(int fd);
        ~Descriptor();

        bool valid() const;
        std::error_code error();

        int fd() const;

        void close(bool withUninstall = true);
        void detach();

        dci::async::Event &readyEvent();
        std::uint_fast32_t readyState() const;
        void resetReadyState(std::uint_fast32_t flags);

    private:
        std::error_code install();
        void uninstall();

    private:
        friend class Engine;
        Descriptor *_nextInEngine{nullptr};
        Descriptor *_prevInEngine{nullptr};
        void setReadyState(std::uint_fast32_t flags);

    private:
        int                 _fd;
        dci::async::Event   _readyEvent;
        std::uint_fast32_t  _readyState{0};
    };
}}}
