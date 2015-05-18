#pragma once

#include "engine.hpp"

namespace dci { namespace poller { namespace impl
{
    class EngineEpoll
        : public Engine
    {
    public:
        EngineEpoll();
        ~EngineEpoll() override;

        virtual bool valid() const override;

        virtual std::error_code installDescriptor(Descriptor *d) override;
        virtual std::error_code uninstallDescriptor(Descriptor *d) override;
    };
}}}
