#pragma once

#include <dci/couple/meta/library.hpp>
#include "signed.hpp"
#include "libraryContent.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Library
        : public Signed
    {
    public:
        Library();
        ~Library();

        const Scope *rootScope() const;

    public:// changes
        LoadResult load(const std::string &file);

        Library &operator=(LibraryContent &&lc);

    private:
        void makeSign();

        LibraryContent _lc;
    };

}}}}
