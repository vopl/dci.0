#pragma once

#include <dci/himpl/layoutProvider.hpp>

namespace dci {namespace couple {namespace runtime {namespace impl {class Hub;}}}}

namespace dci { namespace himpl {
    template <> struct LayoutProvider< ::dci::couple::runtime::impl::Hub > { static const std::size_t _size=0; static const std::size_t _align=1; static const bool _polymorphic=0; };
}}

