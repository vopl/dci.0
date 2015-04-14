#pragma once

#include <dci/himpl/implLayout.hpp>
#include <library.hpp>
#include "libraryContent.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Library
        : public himpl::ImplLayout<Library>
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
