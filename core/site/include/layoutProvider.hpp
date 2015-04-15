#pragma once

#include <dci/himpl/layoutProvider.hpp>

namespace dci {namespace site {namespace impl {class ModulePlace;}}}

namespace dci { namespace himpl {
    template <> struct LayoutProvider< ::dci::site::impl::ModulePlace > { static const std::size_t _size=8; static const std::size_t _align=8; static const bool _polymorphic=0; };
}}

