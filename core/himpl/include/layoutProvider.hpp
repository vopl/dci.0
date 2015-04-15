#pragma once

#include <cstddef>

namespace dci { namespace himpl
{
    template <class T>
    struct LayoutProvider;
}}

#if GENERATE_LAYOUTPROVIDER
#   define LAYOUTPROVIDERFILE(x) <dci/himpl/layoutProviderDefault.hpp>
#else
#   define LAYOUTPROVIDERFILE(x) x
#endif
