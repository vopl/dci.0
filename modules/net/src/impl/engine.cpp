#include "engine.hpp"
#include "host.hpp"
#include "link.hpp"

namespace impl
{
    Engine::Engine(Host *host)
        : _host(host)
    {
    }

    Engine::~Engine()
    {
    }

    const Engine::Links &Engine::links() const
    {
        return _links;
    }

    Host *Engine::host() const
    {
        return _host;
    }

    Link *Engine::addLink(uint32_t id)
    {
        auto &p = _links.insert(std::make_pair(id, std::unique_ptr<Link>())).first->second;
        assert(!p);
        if(!p)
        {
            p.reset(new Link(id));
        }

        return p.get();
    }

    Link *Engine::getLink(uint32_t id)
    {
        auto iter = _links.find(id);
        if(_links.end() != iter)
        {
            return iter->second.get();
        }

        return nullptr;
    }

    void Engine::delLink(uint32_t id)
    {
        _links.erase(id);
    }

}
