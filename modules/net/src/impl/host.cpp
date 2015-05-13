#include "host.hpp"
#include "interface.hpp"
#include "../handlers/host.hpp"
#include <cassert>

namespace impl
{
    Host::Host()
    {
    }

    Host::~Host()
    {
        for(auto h : _handlers)
        {
            h->dropImpl();
        }
        _handlers.clear();
    }

    std::error_code Host::startup()
    {
        assert(!_instance);

        if(!_instance)
        {
            _instance = new Host;
        }

        return std::error_code();
    }

    std::error_code Host::shutdown()
    {
        assert(_instance);

        if(_instance)
        {
            Host *instance = _instance;
            _instance = nullptr;
            delete instance;
        }

        return std::error_code();
    }

    Host *Host::instance()
    {
        return _instance;
    }

    void Host::registerHandler(handlers::Host *handler)
    {
        _handlers.insert(handler);
    }

    void Host::unregisterHandler(handlers::Host *handler)
    {
        _handlers.erase(handler);
    }

    const std::set<std::unique_ptr<Interface>> &Host::interfaces() const
    {
        return _interfaces;
    }

    Host *Host::_instance {nullptr};

}
