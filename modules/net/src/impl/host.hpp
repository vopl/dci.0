#pragma once

#include <system_error>
#include <map>
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

        const std::map<std::string, std::unique_ptr<Interface>> &interfaces() const;

    private:
        void update();

    private:
        static Host *_instance;
        std::set<handlers::Host *> _handlers;

        using Interfaces = std::map<std::string, std::unique_ptr<Interface>>;
        Interfaces _interfaces;
    };
}
