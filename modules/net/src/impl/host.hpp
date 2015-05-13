#pragma once

#include <system_error>
#include <set>
#include <memory>

namespace handlers
{
    class Host;
}

namespace impl
{
    class Interface;

    class Host
    {
    private:
        Host();
        ~Host();

    public:
        static std::error_code startup();
        static std::error_code shutdown();
        static Host *instance();

        void registerHandler(handlers::Host *handler);
        void unregisterHandler(handlers::Host *handler);

        const std::set<std::unique_ptr<Interface>> &interfaces() const;

    private:
        static Host *_instance;
        std::set<handlers::Host *> _handlers;
        std::set<std::unique_ptr<Interface>> _interfaces;
    };
}
