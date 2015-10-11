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

        struct Deleter
        {
            void operator()(LevelNodeBase<Cfg> *);
        };

        using LevelNodeBasePtr = std::unique_ptr<LevelNodeBase, Deleter>;
        using LinkPtr = std::unique_ptr<Link>;

    protected:
        LevelNodeBase();
        ~LevelNodeBase();

    public:
        static LevelNodeBasePtr create(std::size_t level, LevelNodeBasePtr &&child);
        void destroy(std::size_t level);

        LinkId add(std::size_t level, Container *container, LinkPtr &&link);
        Link *get(std::size_t level, const LinkId &id) const;
        LinkPtr del(std::size_t level, Container *container, const LinkId &id);

    private:
        template <class F>
        static auto expandSwitch(std::size_t level, F &&f);
    };


    template <class Cfg>
    using LevelNodeBasePtr = typename LevelNodeBase<Cfg>::LevelNodeBasePtr;


    template <class Cfg, std::size_t level>
    using LevelNodePtr = std::unique_ptr<LevelNode<Cfg, level>>;











    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    void LevelNodeBase<Cfg>::Deleter::operator()(LevelNodeBase<Cfg> *)
    {
        assert(!"never here");
        abort();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    LevelNodeBase<Cfg>::LevelNodeBase()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    LevelNodeBase<Cfg>::~LevelNodeBase()
    {
        //assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    typename LevelNodeBase<Cfg>::LevelNodeBasePtr LevelNodeBase<Cfg>::create(std::size_t level, LevelNodeBasePtr &&child)
    {
        assert(level > 0);
        return expandSwitch(level-1, [&](auto vholder){

            using Child = LevelNode<Cfg, vholder.value>;
            using ChildPtr = LevelNodePtr<Cfg, vholder.value>;

            using This = local::LevelNode<Cfg, vholder.value+1>;
            using ThisPtr = local::LevelNodeBasePtr<Cfg>;

            auto concreteChild = ChildPtr(static_cast<Child *>(child.release()));

            return ThisPtr(new This(std::move(concreteChild)));
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
    LinkId LevelNodeBase<Cfg>::add(std::size_t level, Container *container, LinkPtr &&link)
    {
        return expandSwitch(level, [&](auto vholder){
            return static_cast<LevelNode<Cfg, vholder.value>*>(this)->add(container, std::move(link));
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
    std::unique_ptr<typename LevelNodeBase<Cfg>::Link> LevelNodeBase<Cfg>::del(std::size_t level, Container *container, const LinkId &id)
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
