#pragma once

#include "descriptor.hpp"
#include <memory>
#include <system_error>

namespace dci { namespace poller { namespace impl
{
    class Engine
    {
    public:
        Engine();
        virtual ~Engine();

        virtual bool valid() const = 0;

        virtual std::error_code installDescriptor(Descriptor *d) = 0;
        virtual std::error_code uninstallDescriptor(Descriptor *d) = 0;

    private:
    };

    extern std::unique_ptr<Engine> engine;
}}}
