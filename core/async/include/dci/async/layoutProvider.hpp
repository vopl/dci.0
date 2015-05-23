#pragma once

#include <dci/himpl/layoutProvider.hpp>

namespace dci {namespace async {namespace impl {class Scheduler;}}}
namespace dci {namespace async {namespace impl {class Task;}}}
namespace dci {namespace async {namespace impl {class Mutex;}}}
namespace dci {namespace async {namespace impl {class Event;}}}
namespace dci {namespace async {namespace impl {class Pulser;}}}

namespace dci { namespace himpl {
    template <> struct LayoutProvider< ::dci::async::impl::Scheduler > { static const std::size_t _size=992; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::async::impl::Task > { static const std::size_t _size=32; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::async::impl::Mutex > { static const std::size_t _size=40; static const std::size_t _align=8; static const bool _polymorphic=true; };
    template <> struct LayoutProvider< ::dci::async::impl::Event > { static const std::size_t _size=40; static const std::size_t _align=8; static const bool _polymorphic=true; };
    template <> struct LayoutProvider< ::dci::async::impl::Pulser > { static const std::size_t _size=40; static const std::size_t _align=8; static const bool _polymorphic=true; };
}}

