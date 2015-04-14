#pragma once

#include <library.hpp>
#include "libraryContent.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Library
    {
    public:
        Library();
        ~Library();

        const Scope *rootScope() const;

    public:// changes
        LoadResult load(const std::string &file);

        Library &operator=(LibraryContent &&lc);

    private:
        LibraryContent _lc;
    };

}}}}
