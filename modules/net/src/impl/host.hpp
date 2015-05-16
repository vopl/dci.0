#pragma once

#include "engine.hpp"
#include <system_error>
#include <set>

namespace handlers
{
    class Host;
}

namespace impl
{
    class Link;

    class Host
    {
    private:
        Host();

    public:
        ~Host();

        static dci::async::Future<std::error_code> startup();
        static dci::async::Future<std::error_code> shutdown();
        static Host *instance();

        bool valid() const;

        void registerHandler(handlers::Host *handler);
        void unregisterHandler(handlers::Host *handler);

        const Engine::Links &links() const;

        void onLinkAdded(Link *link);

    private:
        static std::unique_ptr<Host> _instance;

        using Handlers = std::set<handlers::Host *>;
        Handlers _handlers;

        std::unique_ptr<Engine> _engine;
    private:
    };
}
