#pragma once

#include <dci/himpl/layoutProvider.hpp>

namespace dci {namespace async {namespace impl {class Scheduler;}}}
namespace dci {namespace async {namespace impl {class Task;}}}
namespace dci {namespace async {namespace impl {class Waitable;}}}
namespace dci {namespace async {namespace impl {class Lockable;}}}
namespace dci {namespace async {namespace impl {class Waiter;}}}
namespace dci {namespace async {namespace impl {class Mutex;}}}
namespace dci {namespace async {namespace impl {class MutexRecursive;}}}
namespace dci {namespace async {namespace impl {class Semaphore;}}}
namespace dci {namespace async {namespace impl {class Event;}}}
namespace dci {namespace async {namespace impl {class NotifierOne;}}}
namespace dci {namespace async {namespace impl {class NotifierAll;}}}
namespace dci {namespace async {namespace impl {class PulserOne;}}}
namespace dci {namespace async {namespace impl {class PulserAll;}}}
namespace dci {namespace async {namespace impl {class Barrier;}}}

namespace dci { namespace himpl {
    template <> struct LayoutProvider< ::dci::async::impl::Scheduler > { static const std::size_t _size=80; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::async::impl::Task > { static const std::size_t _size=32; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::async::impl::Waitable > { static const std::size_t _size=48; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::async::impl::Lockable > { static const std::size_t _size=0; static const std::size_t _align=1; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::async::impl::Waiter > { static const std::size_t _size=40; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::async::impl::Mutex > { static const std::size_t _size=56; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::async::impl::MutexRecursive > { static const std::size_t _size=64; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::async::impl::Semaphore > { static const std::size_t _size=56; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::async::impl::Event > { static const std::size_t _size=56; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::async::impl::NotifierOne > { static const std::size_t _size=56; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::async::impl::NotifierAll > { static const std::size_t _size=56; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::async::impl::PulserOne > { static const std::size_t _size=56; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::async::impl::PulserAll > { static const std::size_t _size=56; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::async::impl::Barrier > { static const std::size_t _size=64; static const std::size_t _align=8; static const bool _polymorphic=false; };
}}

