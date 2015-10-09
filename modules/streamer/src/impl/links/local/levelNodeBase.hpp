#pragma once
#include <dci/couple/runtime.hpp>
#include "linkId.hpp"
#include "switchExpander.hpp"

namespace impl { namespace links {
    using namespace dci::couple::runtime;
    using namespace streamer;

    template <class Link> class Local;
}}

namespace impl { namespace links { namespace local
{
    template <class Link, std::size_t width, std::size_t levels, std::size_t level> class LevelNode;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t width, std::size_t levels>
    class LevelNodeBase
    {
    public:
        LevelNodeBase(const LinkId &idOffset);
        ~LevelNodeBase();


        LinkId add(std::size_t level, Local<Link> *container, std::unique_ptr<Link> &&link);
        Link *get(std::size_t level, const LinkId &id);
        std::unique_ptr<Link> del(std::size_t level, Local<Link> *container, const LinkId &id);

    protected:

        LinkId _idOffset;
    };

    template <class Link, std::size_t width, std::size_t levels>
    using LevelNodeBasePtr = std::unique_ptr<LevelNodeBase<Link, width, levels>>;










    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t width, std::size_t levels>
    LevelNodeBase<Link, width, levels>::LevelNodeBase(const LinkId &idOffset)
        : _idOffset(idOffset)
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t width, std::size_t levels>
    LevelNodeBase<Link, width, levels>::~LevelNodeBase()
    {
        //assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t width, std::size_t levels>
    LinkId LevelNodeBase<Link, width, levels>::add(std::size_t level, Local<Link> *container, std::unique_ptr<Link> &&link)
    {
        return SwitchExpander<std::make_index_sequence<levels>>::exec(level, [&](auto vholder){
            return static_cast<LevelNode<Link, width, levels, vholder.value>*>(this)->add(container, std::move(link));
        });
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t width, std::size_t levels>
    Link *LevelNodeBase<Link, width, levels>::get(std::size_t level, const LinkId &id)
    {
        return SwitchExpander<std::make_index_sequence<levels>>::exec(level, [&](auto vholder){
            return static_cast<LevelNode<Link, width, levels, vholder.value>*>(this)->get(id);
        });
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t width, std::size_t levels>
    std::unique_ptr<Link> LevelNodeBase<Link, width, levels>::del(std::size_t level, Local<Link> *container, const LinkId &id)
    {
        return SwitchExpander<std::make_index_sequence<levels>>::exec(level, [&](auto vholder){
            return static_cast<LevelNode<Link, width, levels, vholder.value>*>(this)->del(container, id);
        });
    }


}}}
