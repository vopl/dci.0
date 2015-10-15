#pragma once
#include <dci/couple/runtime.hpp>
#include "linkId.hpp"
#include "switchExpander.hpp"

namespace impl { namespace links { namespace local
{
    template <class Cfg, std::size_t level> class LevelNode;

    template <class Cfg> class LevelNodeBase;


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    class LevelNodeBase
    {
    public:
        using Container = typename Cfg::Container;
        using Link = typename Cfg::Link;
        static const std::size_t _width = Cfg::_width;
        static const std::size_t _levels = Cfg::_levels;

    protected:
        LevelNodeBase();
        ~LevelNodeBase();

    public:
        static LevelNodeBase *create(std::size_t level, LevelNodeBase *child);
        void destroy(std::size_t level);

        LinkId add(std::size_t level, Container *container, Link *link);
        bool add(std::size_t level, Container *container, LinkId id, Link *link);
        Link *get(std::size_t level, const LinkId &id) const;
        Link *del(std::size_t level, Container *container, const LinkId &id);

    private:
        template <class F>
        static auto expandSwitch(std::size_t level, F &&f);
    };













    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    LevelNodeBase<Cfg>::LevelNodeBase()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    LevelNodeBase<Cfg>::~LevelNodeBase()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    LevelNodeBase<Cfg> *LevelNodeBase<Cfg>::create(std::size_t level, LevelNodeBase *child)
    {
        assert(level > 0);
        return expandSwitch(level-1, [&](auto vholder){

            using Child = LevelNode<Cfg, vholder.value>;
            using This = local::LevelNode<Cfg, vholder.value+1>;

            Child *concreteChild = static_cast<Child *>(child);
            return static_cast<LevelNodeBase<Cfg> *>(new This(concreteChild));
        });
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    void LevelNodeBase<Cfg>::destroy(std::size_t level)
    {
        expandSwitch(level, [&](auto vholder){
            delete static_cast<LevelNode<Cfg, vholder.value>*>(this);
        });
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    LinkId LevelNodeBase<Cfg>::add(std::size_t level, Container *container, Link *link)
    {
        return expandSwitch(level, [&](auto vholder){
            return static_cast<LevelNode<Cfg, vholder.value>*>(this)->add(container, link);
        });
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    bool LevelNodeBase<Cfg>::add(std::size_t level, Container *container, LinkId id, Link *link)
    {
        return expandSwitch(level, [&](auto vholder){
            return static_cast<LevelNode<Cfg, vholder.value>*>(this)->add(container, id, link);
        });
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    typename LevelNodeBase<Cfg>::Link *LevelNodeBase<Cfg>::get(std::size_t level, const LinkId &id) const
    {
        return expandSwitch(level, [&](auto vholder){
            return static_cast<const LevelNode<Cfg, vholder.value>*>(this)->get(id);
        });
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    typename LevelNodeBase<Cfg>::Link *LevelNodeBase<Cfg>::del(std::size_t level, Container *container, const LinkId &id)
    {
        return expandSwitch(level, [&](auto vholder){
            return static_cast<LevelNode<Cfg, vholder.value>*>(this)->del(container, id);
        });
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    template <class F>
    auto LevelNodeBase<Cfg>::expandSwitch(std::size_t level, F &&f)
    {
        return SwitchExpander<std::make_index_sequence<_levels>>::exec(level, std::move(f));
    }


}}}
