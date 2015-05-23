#include <dci/poll/functions.hpp>
#include <dci/poll/error.hpp>
#include "impl/engine.hpp"

namespace dci { namespace poll
{
    std::error_code initialize()
    {
        return impl::Engine::initialize();
    }

    std::error_code run()
    {
        if(!impl::engine)
        {
            return err_general::not_initialized;
        }

        return impl::engine->run();
    }

    std::error_code stop()
    {
        if(!impl::engine)
        {
            return err_general::not_initialized;
        }

        return impl::engine->stop();
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
