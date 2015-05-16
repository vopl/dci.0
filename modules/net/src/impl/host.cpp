#include "host.hpp"
#include "link.hpp"
#include "../handlers/host.hpp"
#include "../handlers/link.hpp"

#include "engineRtnetlink.hpp"

#include <dci/logger.hpp>
#include <dci/async.hpp>

//#include <cassert>
//#include <cstring>

namespace impl
{
    Host::Host()
    {
        if(!_engine)
        {
            _engine.reset(new Rtnetlink(this));

            if(!_engine->valid())
            {
                _engine.reset();
            }
        }
    }

    Host::~Host()
    {
        for(auto h : _handlers)
        {
            h->dropImpl();
        }
        _handlers.clear();
        _engine.reset();
    }

    dci::async::Future<std::error_code> Host::startup()
    {
        assert(!_instance);

        return dci::async::spawn([](dci::async::Promise<std::error_code> p){
            Host *host = new Host;
            if(!host->valid())
            {
                delete host;
                p.resolveError(std::make_error_code(std::errc::network_unreachable));
                return;
            }

            if(_instance)
            {
                delete host;
                p.resolveError(std::make_error_code(std::errc::already_connected));
                return;
            }

            _instance.reset(host);
            p.resolveValue();
        });
    }

    dci::async::Future<std::error_code> Host::shutdown()
    {
        assert(_instance);

        return dci::async::spawn([](dci::async::Promise<std::error_code> p){
           _instance.reset();
           p.resolveValue();
        });
    }

    Host *Host::instance()
    {
        return _instance.get();
    }

    bool Host::valid() const
    {
        return _engine && _engine->valid();
    }

    void Host::registerHandler(handlers::Host *handler)
    {
        _handlers.insert(handler);
    }

    void Host::unregisterHandler(handlers::Host *handler)
    {
        _handlers.erase(handler);
    }

    const Engine::Links &Host::links() const
    {
        if(_engine)
        {
            return _engine->links();
        }

        static Engine::Links stub;
        return stub;
    }

    void Host::onLinkAdded(Link *link)
    {
        for(handlers::Host *h : _handlers)
        {
            net::Link linkIface(* new handlers::Link(link));

            dci::async::spawn([linkIface(std::move(linkIface)), h]() mutable
            {
                h->linkAdded(std::move(linkIface));
            });

            //dci::async::spawn(&handlers::HostOpposite::linkAdded, std::move(h), std::move(linkIface));
        }
    }

    std::unique_ptr<Host> Host::_instance {nullptr};

}
