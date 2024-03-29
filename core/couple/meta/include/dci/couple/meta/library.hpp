#pragma once

#include "signed.hpp"
#include "alias.hpp"
#include "array.hpp"
#include "enum.hpp"
#include "errc.hpp"
#include "interface.hpp"
#include "list.hpp"
#include "map.hpp"
#include "ptr.hpp"
#include "primitive.hpp"
#include "scope.hpp"
#include "set.hpp"
#include "struct.hpp"
#include "variant.hpp"

#include <string>
#include <vector>
#include <system_error>

namespace dci { namespace couple { namespace meta
{
    enum class LoadResult
    {
        ok,
        corruptedFile,
        malformedFile,
    };

    class APIDCI_COUPLE_META Library
        : public himpl::FaceLayout<impl::Library, Signed>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Library();
        ~Library();

        const Scope *rootScope() const;

    public:// changes
        LoadResult load(const std::string &file);
        //save
    };
}}}
