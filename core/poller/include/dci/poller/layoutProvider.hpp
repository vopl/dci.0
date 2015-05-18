#pragma once

#include <dci/himpl/layoutProvider.hpp>

namespace dci {namespace poller {namespace impl {class Descriptor;}}}

namespace dci { namespace himpl {
    template <> struct LayoutProvider< ::dci::poller::impl::Descriptor > { static const std::size_t _size=72; static const std::size_t _align=8; static const bool _polymorphic=false; };
}}

