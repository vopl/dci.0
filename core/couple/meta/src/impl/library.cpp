#include "library.hpp"
#include <dci/couple/runtime/signBuilder.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    Library::Library()
    {
    }

    Library::~Library()
    {
    }

    const Scope *Library::rootScope() const
    {
        for(Scope *s : _lc.items<Scope>())
        {
            if(!s->scope())
            {
                return s;
            }
        }

        return nullptr;
    }

    LoadResult Library::load(const std::string &file)
    {
        (void)file;
        return LoadResult::malformedFile;
    }

    Library &Library::operator=(LibraryContent &&lc)
    {
        _lc = std::forward<LibraryContent>(lc);
        makeSign();
        makeSign();
        return *this;
    }

    namespace
    {
        template <class T>
        void makeSign4Items(const std::set<T *> &items)
        {
            for(T *v : items)
            {
                v->makeSign();
            }
        }

        template <class T>
        void collectSign4Items(runtime::SignBuilder &s, const std::set<T *> &items)
        {
            for(T *v : items)
            {
                s.add(v->sign());
            }
        }
    }

    void Library::makeSign()
    {
        makeSign4Items(_lc.items<Primitive>());

        makeSign4Items(_lc.items<Scope>());//structs, variants, ifaces,

        makeSign4Items(_lc.items<List>());
        makeSign4Items(_lc.items<Set>());
        makeSign4Items(_lc.items<Map>());
        makeSign4Items(_lc.items<Ptr>());
        makeSign4Items(_lc.items<Array>());

        makeSign4Items(_lc.items<Method>());
        makeSign4Items(_lc.items<Attribute>());
        makeSign4Items(_lc.items<EnumValue>());

        makeSign4Items(_lc.items<Alias>());
        makeSign4Items(_lc.items<Enum>());

        runtime::SignBuilder s;

        collectSign4Items(s, _lc.items<Scope>());
        collectSign4Items(s, _lc.items<List>());
        collectSign4Items(s, _lc.items<Set>());
        collectSign4Items(s, _lc.items<Map>());
        collectSign4Items(s, _lc.items<Ptr>());
        collectSign4Items(s, _lc.items<Array>());
        collectSign4Items(s, _lc.items<Alias>());
        collectSign4Items(s, _lc.items<Enum>());

        setSign(s.finish());
    }

}}}}
