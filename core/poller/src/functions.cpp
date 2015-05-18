#include <dci/poller/functions.hpp>
#include "impl/engine.hpp"

namespace dci { namespace poller
{
    std::error_code initialize()
    {
        return impl::Engine::initialize();
    }

    std::error_code execute(std::int32_t timeoutms)
    {
        assert(impl::engine);
        if(!impl::engine)
        {
            abort();
        }

        return impl::engine->execute(timeoutms);
    }

    std::error_code interrupt()
    {
        assert(impl::engine);
        if(!impl::engine)
        {
            abort();
        }

        return impl::engine->interrupt();
    }

    std::error_code deinitialize()
    {
        assert(impl::engine);
        if(!impl::engine)
        {
            abort();
        }

        return impl::engine->deinitialize();
    }

}}
