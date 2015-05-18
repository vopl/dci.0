#include "engine.hpp"
#include "engineEpoll.hpp"

namespace dci { namespace poller { namespace impl
{
    Engine::Engine()
    {
    }

    Engine::~Engine()
    {
    }

    std::unique_ptr<Engine> engine;
}}}
