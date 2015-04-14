#include "library.hpp"

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
            if(std::string() == s->name())
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
        return *this;
    }

}}}}
