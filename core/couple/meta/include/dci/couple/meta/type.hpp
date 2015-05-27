#pragma once

#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")
#include <dci/himpl/faceLayout.hpp>

#include "api.hpp"
#include <type_traits>
#include <cstdlib>
#include <cassert>

namespace dci { namespace couple { namespace meta
{
    enum class TypeConcrete
    {
        null,
        alias,
        array,
        enum_,
        errc,
        iface,
        list,
        map,
        primitive,
        ptr,
        set,
        struct_,
        variant
    };

    class APIDCI_COUPLE_META Type
        : public himpl::FaceLayout<impl::Type>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Type() = delete;
        ~Type() = delete;

        TypeConcrete concrete() const;
    };
}}}
