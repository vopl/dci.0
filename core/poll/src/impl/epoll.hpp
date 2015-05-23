#pragma once

#include "engine.hpp"

namespace dci { namespace poll { namespace impl
{
    class Epoll
        : public Engine
    {
    public:
        Epoll();
        ~Epoll() override;

        virtual std::error_code startup() override;

        virtual std::error_code execute(std::chrono::milliseconds timeout) override;
        virtual std::error_code interrupt() override;

        virtual std::error_code installDescriptor(Descriptor *d) override;
        virtual std::error_code uninstallDescriptor(Descriptor *d) override;

    private:
        int _fd {-1};
    };
}}}
