#pragma once

#include <dci/himpl/layoutProvider.hpp>

namespace dci {namespace site {namespace impl {class ModulePlace;}}}
namespace dci {namespace site {namespace impl {class Instance;}}}

namespace dci { namespace himpl {
    template <> struct LayoutProvider< ::dci::site::impl::ModulePlace > { static const std::size_t _size=8; static const std::size_t _align=8; static const bool _polymorphic=0; };
    template <> struct LayoutProvider< ::dci::site::impl::Instance > { static const std::size_t _size=32; static const std::size_t _align=8; static const bool _polymorphic=0; };
}}

