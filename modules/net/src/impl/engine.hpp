#pragma once

#include <dci/async.hpp>
#include <map>
#include <set>
#include <memory>

namespace handlers
{
    class Host;
}

namespace impl
{
    class Link;
    class Host;

    class Engine
    {
    public:
        using Links = std::map<int32_t, std::unique_ptr<Link>>;

    public:
        Engine(Host *host);
        virtual ~Engine();

        virtual bool valid() const = 0;
        const Links &links() const;
        Host *host() const;

    protected:
        Link *addLink(int32_t id);
        Link *getLink(int32_t id);
        void delLink(int32_t id);

    private:
        Host *_host;
        Links _links;
    };
}
