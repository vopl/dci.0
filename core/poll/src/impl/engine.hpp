#pragma once

#include "descriptor.hpp"
#include "timerEngine.hpp"
#include <memory>
#include <system_error>
#include <chrono>

namespace dci { namespace poll { namespace impl
{
    class Engine
    {
    protected:
        Engine();

    public:
        virtual ~Engine();

        static std::error_code initialize();
        std::error_code deinitialize();

        std::error_code run();
        std::error_code stop();

        TimerEngine &timerEngine();

        virtual std::error_code startup() = 0;
        virtual std::error_code execute(std::chrono::milliseconds timeout) = 0;
        virtual std::error_code interrupt() = 0;
        virtual std::error_code installDescriptor(Descriptor *d) = 0;
        virtual std::error_code uninstallDescriptor(Descriptor *d) = 0;

    protected:
        static void addReadyState(Descriptor *d, std::uint_fast32_t state);

        void registerDescriptor(Descriptor *d);
        bool deregisterDescriptor(Descriptor *d);

    private:
        Descriptor *_descriptors{nullptr};
        TimerEngine _timerEngine;
        bool        _stop{true};
    };

    extern std::unique_ptr<Engine> engine;
}}}
