#pragma once

#include <dci/himpl/layoutProvider.hpp>

namespace dci {namespace io {namespace impl {class Stream;}}}
namespace dci {namespace io {namespace impl {class Client;}}}
namespace dci {namespace io {namespace impl {class Server;}}}
namespace dci {namespace io {namespace impl {class Data;}}}

namespace dci { namespace himpl {
    template <> struct LayoutProvider< ::dci::io::impl::Stream > { static const std::size_t _size=16; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::io::impl::Client > { static const std::size_t _size=0; static const std::size_t _align=1; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::io::impl::Server > { static const std::size_t _size=56; static const std::size_t _align=8; static const bool _polymorphic=true; };
    template <> struct LayoutProvider< ::dci::io::impl::Data > { static const std::size_t _size=24; static const std::size_t _align=8; static const bool _polymorphic=false; };
}}

