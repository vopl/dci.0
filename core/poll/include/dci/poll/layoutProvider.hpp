#pragma once

#include <dci/himpl/layoutProvider.hpp>

namespace dci {namespace poll {namespace impl {class Descriptor;}}}
namespace dci {namespace poll {namespace impl {class Timer;}}}

namespace dci { namespace himpl {
    template <> struct LayoutProvider< ::dci::poll::impl::Descriptor > { static const std::size_t _size=104; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::poll::impl::Timer > { static const std::size_t _size=104; static const std::size_t _align=8; static const bool _polymorphic=false; };
}}

